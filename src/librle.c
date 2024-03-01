/*  librle.c */

#include "librle.h"

unsigned char * RLE_Compress(const unsigned char * DATA, const size_t DATA_LENGTH, size_t * compressed_data_length_ptr)
{
    if (!DATA || DATA_LENGTH == 0 || !compressed_data_length_ptr) return NULL;

    *compressed_data_length_ptr = 0;
    unsigned char * compressed_data_buffer = (char *) malloc(DATA_LENGTH);
    if (!compressed_data_buffer) return NULL;

    size_t compressed_data_buffer_size = DATA_LENGTH;

    size_t w = 0, r = 0;
    while (r < DATA_LENGTH) {
        size_t re = r;
        while (DATA[re] == DATA[r] && re - r < 256) re++;
        if (re > r + 1) {
            if (w > compressed_data_buffer_size - 2) {
                compressed_data_buffer_size += LIBRLE_BUFFER_BLOCK_SIZE;
                compressed_data_buffer = (unsigned char *) realloc(compressed_data_buffer, compressed_data_buffer_size);
            }
            compressed_data_buffer[w++] = 1;
            compressed_data_buffer[w++] = DATA[r];
            compressed_data_buffer[w++] = re - r;
            r = re;
        }
        if (r < DATA_LENGTH - 1 && DATA[r] != DATA[r + 1]) {
            compressed_data_buffer[w++] = 0;
            while (re < DATA_LENGTH && re - r < 256 && DATA[re] != DATA[re + 1]) {
                compressed_data_buffer[++w] = DATA[re++];
                if (w >= compressed_data_buffer_size) {
                    compressed_data_buffer_size += LIBRLE_BUFFER_BLOCK_SIZE;
                    compressed_data_buffer = (unsigned char *) realloc(compressed_data_buffer, compressed_data_buffer_size);
                }
            }
            compressed_data_buffer[w - (re - r)] = re - r;
            w++;
            r = re;
        }
    }
    *compressed_data_length_ptr = w;
    return realloc(compressed_data_buffer, w);
}

unsigned char * RLE_Decompress(const unsigned char * RLE_DATA, const size_t RLE_DATA_LENGTH, size_t * uncompressed_data_length_ptr)
{
    if (!RLE_DATA || !uncompressed_data_length_ptr || RLE_DATA_LENGTH == 0) return NULL;

    *uncompressed_data_length_ptr = 0;

    size_t uncompressed_data_size = LIBRLE_BUFFER_BLOCK_SIZE;
    unsigned char * uncompressed_data = (unsigned char *) malloc(uncompressed_data_size);
    if (!uncompressed_data) return NULL;

    size_t r = 0, w = 0;

    while (r < RLE_DATA_LENGTH) {
        switch (RLE_DATA[r++]) {
            case 0 : {
                size_t end_of_chars = r + RLE_DATA[r++];
                while (r < end_of_chars) {
                    uncompressed_data[w++] = RLE_DATA[r++];
                    if (w == uncompressed_data_size) {
                        uncompressed_data_size += LIBRLE_BUFFER_BLOCK_SIZE;
                        uncompressed_data = (unsigned char *) realloc(uncompressed_data, uncompressed_data_size);
                    }
                }
            } break;
            default : {
                unsigned char char_to_repeat = RLE_DATA[r++];
                size_t number_of_times = RLE_DATA[r++];
                for (size_t n = 0; n < number_of_times; n++) {
                    uncompressed_data[w++] = char_to_repeat;
                    if (w == uncompressed_data_size) {
                        uncompressed_data_size += LIBRLE_BUFFER_BLOCK_SIZE;
                        uncompressed_data = (unsigned char *) realloc(uncompressed_data, uncompressed_data_size);
                    }
                }
            }
        }
    }

    *uncompressed_data_length_ptr = w;
    return realloc(uncompressed_data, w);
}