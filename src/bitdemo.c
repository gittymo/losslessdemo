/*  bitdemo.c
    Demonstration of bitstreams
    (C)2024 Morgan Evans */

#include "libbit.h"

int main(int argc, char * argv[])
{
    BitBuffer * bit_buffer = BitBuffer_Create();
    if (!bit_buffer) exit(EXIT_FAILURE);

    bit_buffer->bit_pos = 6;
    BitBuffer_WriteBits(17, 5, bit_buffer);

    free(bit_buffer);
    exit(EXIT_SUCCESS);
}