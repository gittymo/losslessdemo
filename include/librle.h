/* librle.h */

#ifndef COM_PLUS_MEVANSPN_LIBRLE
#define COM_PLUS_MEVANSPN_LIBRLE

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define LIBRLE_BUFFER_BLOCK_SIZE 65536

char * RLE_Compress(const char * DATA, const size_t DATA_LENGTH, size_t * compressed_data_length_ptr);

#endif