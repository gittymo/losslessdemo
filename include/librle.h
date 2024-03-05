/* librle.h */

#ifndef COM_PLUS_MEVANSPN_LIBRLE
#define COM_PLUS_MEVANSPN_LIBRLE

#include <stdio.h>
#include <stdlib.h>

#define LIBRLE_BUFFER_BLOCK_SIZE 65536

unsigned char * RLE_Compress(const unsigned char * DATA, const size_t DATA_LENGTH, size_t * compressed_data_length_ptr);
unsigned char * RLE_Decompress(const unsigned char * RLE_DATA, const size_t RLE_DATA_LENGTH, size_t * uncompressed_data_length_ptr);

#endif