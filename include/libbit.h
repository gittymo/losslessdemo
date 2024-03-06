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
    u_char * buffer;
    size_t _buffer_size;
    size_t byte_pos, alt_byte_pos;
    u_char bit_pos, alt_bit_pos;
    bool is_read_buffer;
} BitBuffer;

BitBuffer * BitBuffer_Create();
bool BitBuffer_WriteBits(const size_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer);
bool BitBuffer_WriteByte(const u_char VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer);
bool BitBuffer_WriteShort(const u_int16_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer);
bool BitBuffer_WriteInt(const u_int32_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer);
bool BitBuffer_WriteLong(const u_int64_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer);
bool BitBuffer_WriteBytes(const u_char * DATA, const size_t DATA_LENGTH_BYTES, BitBuffer * bit_buffer);
bool BitBuffer_WriteString(const char * STRING, const size_t STRING_LENGTH, BitBuffer * bit_buffer);
void BitBuffer_Free(BitBuffer * bit_buffer);

size_t _BitBuffer_getByteLengthForBitCount(const u_char BIT_COUNT);
void _BitBuffer_RevertData(const size_t BYTES_TO_REVERT, const u_char FIRST_ORIGINAL_BYTE_VALUE, const u_char FIRST_ORIGINAL_BYTE_BIT_POS, BitBuffer * bit_buffer);
void _BitBuffer_SwapMode(BitBuffer * bit_buffer);

#endif