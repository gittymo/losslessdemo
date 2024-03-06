/* libbit.c
    A small library for reading and writing variable bit length data.
    (C)2024 Morgan Evans */

#include "libbit.h"

BitBuffer * BitBuffer_Create()
{
    BitBuffer * bit_buffer = (BitBuffer *) malloc(sizeof(BitBuffer));
    if (!bit_buffer) return NULL;
    bit_buffer->data = (u_char *) malloc(LIBBIT_DEFAULT_BUFFER_SIZE_BYTES);
    if (!bit_buffer->data) {
        free(bit_buffer);
        return NULL;
    }
    bit_buffer->size = LIBBIT_DEFAULT_BUFFER_SIZE_BYTES;
    bit_buffer->byte_pos = 0;
    bit_buffer->bit_pos;
    return bit_buffer;
}

size_t _BitBuffer_getByteLengthForBitCount(const u_char BIT_COUNT)
{
    return (BIT_COUNT / 8) + (((BIT_COUNT / 8) * 8) < BIT_COUNT);
}

bool BitBuffer_WriteBits(const size_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer)
{
    if (!bit_buffer) return false;
    const u_char VALUE_BIT_LENGTH = BITS_TO_WRITE == 0 || BITS_TO_WRITE > 64 ? 8 : BITS_TO_WRITE;
    const size_t VALUE_BYTE_LENGTH = _BitBuffer_getByteLengthForBitCount(VALUE_BIT_LENGTH);
    
    u_char * value_byte_data = (u_char *) &VALUE;
    size_t value_bits_left_to_write = VALUE_BIT_LENGTH;
    for (size_t b = 0; b < VALUE_BYTE_LENGTH; b++)
    {   
        if (!BitBuffer_WriteByte(value_byte_data[b], value_bits_left_to_write >= 8 ? 8 : value_bits_left_to_write, bit_buffer)) return false;
        value_bits_left_to_write -= 8;
    }
    return true;
}

bool BitBuffer_WriteByte(const u_char VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer)
{
    if (!bit_buffer) return false;
    const u_char VALUE_BIT_LENGTH = BITS_TO_WRITE == 0 || BITS_TO_WRITE > 8 ? 8 : BITS_TO_WRITE;
    const u_char LOW_VALUE_BYTE_FREE_BITS = 8 - bit_buffer->bit_pos;
    const u_char LOW_BITS_TO_WRITE = VALUE_BIT_LENGTH > LOW_VALUE_BYTE_FREE_BITS ? LOW_VALUE_BYTE_FREE_BITS : VALUE_BIT_LENGTH;
    const u_char LOW_BITS_VALUE = (VALUE & ((1 << LOW_BITS_TO_WRITE) - 1)) << bit_buffer->bit_pos;
    bit_buffer->data[bit_buffer->byte_pos++] += LOW_BITS_VALUE;
    
    if (bit_buffer->byte_pos == bit_buffer->size) {
        u_char * bigger_buffer = (u_char *) realloc(bit_buffer->data, bit_buffer->size + LIBBIT_DEFAULT_BUFFER_SIZE_BYTES);
        if (!bigger_buffer) return false;
        bit_buffer->data = bigger_buffer;
        bit_buffer->size += LIBBIT_DEFAULT_BUFFER_SIZE_BYTES;
    }

    const u_char HIGH_BITS_TO_WRITE = VALUE_BIT_LENGTH - LOW_BITS_TO_WRITE;
    if (HIGH_BITS_TO_WRITE == 0) return true;
    
    const u_char HIGH_BIT_VALUE = (VALUE & (((1 << HIGH_BITS_TO_WRITE) - 1) <<  LOW_BITS_TO_WRITE)) >> LOW_BITS_TO_WRITE;
    bit_buffer->data[bit_buffer->byte_pos] = HIGH_BIT_VALUE;
    bit_buffer->bit_pos = HIGH_BITS_TO_WRITE;
    return true;
}

bool BitBuffer_WriteShort(const u_int16_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer)
{
    if (!bit_buffer) return false;
    const u_char VALUE_BIT_LENGTH = BITS_TO_WRITE == 0 || BITS_TO_WRITE > 16 ? 16 : BITS_TO_WRITE;
    return BitBuffer_WriteBits(VALUE, VALUE_BIT_LENGTH, bit_buffer);
}

bool BitBuffer_WriteInt(const u_int32_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer)
{
    if (!bit_buffer) return false;
    const u_char VALUE_BIT_LENGTH = BITS_TO_WRITE == 0 || BITS_TO_WRITE > 32 ? 32 : BITS_TO_WRITE;
    return BitBuffer_WriteBits(VALUE, VALUE_BIT_LENGTH, bit_buffer);
}

bool BitBuffer_WriteLong(const u_int64_t VALUE, const u_char BITS_TO_WRITE, BitBuffer * bit_buffer)
{
    if (!bit_buffer) return false;
    const u_char VALUE_BIT_LENGTH = BITS_TO_WRITE == 0 || BITS_TO_WRITE > 64 ? 64 : BITS_TO_WRITE;
    return BitBuffer_WriteBits(VALUE, VALUE_BIT_LENGTH, bit_buffer);
}

void BitBuffer_Free(BitBuffer * bit_buffer)
{   
    if (!bit_buffer) return;
    if (bit_buffer->data) free(bit_buffer->data);
    bit_buffer->data = NULL;
    bit_buffer->bit_pos = 0;
    bit_buffer->byte_pos = 0;
    bit_buffer->size = 0;
    free(bit_buffer);
}

int main(int argc, char * argv[])
{
    BitBuffer * bit_buffer = BitBuffer_Create();
    if (!bit_buffer) exit(EXIT_FAILURE);

    bit_buffer->bit_pos = 6;
    BitBuffer_WriteBits(17, 5, bit_buffer);

    free(bit_buffer);
    exit(EXIT_SUCCESS);
}
