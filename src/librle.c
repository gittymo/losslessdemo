/*  librle.c */

#include "librle.h"

char * RLE_Compress(const char * DATA, const size_t DATA_LENGTH, size_t * compressed_data_length_ptr)
{
    if (!DATA || DATA_LENGTH == 0 || !compressed_data_length_ptr) return NULL;

    *compressed_data_length_ptr = 0;
    char * compressed_data_buffer = (char *) malloc(DATA_LENGTH);
    if (!compressed_data_buffer) return NULL;

    size_t compressed_data_buffer_size = DATA_LENGTH;

    size_t w = 0, r = 0;
    while (r < DATA_LENGTH) {
        size_t re = r;
        while (DATA[re] == DATA[r] && re - r < 256) re++;
        if (re > r + 1) {
            if (w > compressed_data_buffer_size - 2) {
                compressed_data_buffer_size += LIBRLE_BUFFER_BLOCK_SIZE;
                compressed_data_buffer = (char *) realloc(compressed_data_buffer, compressed_data_buffer_size);
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
                    compressed_data_buffer = (char *) realloc(compressed_data_buffer, compressed_data_buffer_size);
                }
            }
            compressed_data_buffer[w - (re - r)] = re - r;
            w++;
            r = re;
        }
    }
    *compressed_data_length_ptr = w;
    return compressed_data_buffer;
}