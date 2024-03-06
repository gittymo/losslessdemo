/* libbit.h
    A small library for reading and writing variable bit length data.
    (C)2024 Morgan Evans */

#ifndef COM_PLUS_MEVANSPN_LIBBIT
#define COM_PLUS_MEVANSPN_LIBBIT

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define LIBBIT_DEFAULT_BUFFER_SIZE_BYTES 1024

typedef struct {
    u_char * data;
    size_t size;
    size_t byte_pos;
    u_char bit_pos;
} BitBuffer;

BitBuffer * BitBuffer_Create();
bool BitBuffer_WriteBits(const size_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer);
bool BitBuffer_WriteByte(const u_char VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer);
bool BitBuffer_WriteShort(const u_int16_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer);
bool BitBuffer_WriteInt(const u_int32_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer);
bool BitBuffer_WriteLong(const u_int64_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer);
void BitBuffer_Free(BitBuffer * bit_buffer);

size_t _BitBuffer_getByteLengthForBitCount(const u_char BIT_COUNT);

#endif