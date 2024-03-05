/*  libcodetable.h */

#ifndef COM_PLUS_MEVANSPN_LIBCODETABLE
#define COM_PLUS_MEVANSPN_LIBCODETABLE

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define LIBCODETABLE_ENTRIES_BLOCK_SIZE 16

typedef struct {
    size_t code;
    size_t code_length_bits;
    char * data;
    size_t data_length_bytes;
} _CodeTableEntry; 

typedef struct {
    _CodeTableEntry ** entries;
    size_t entries_count;
    size_t entries_array_size;
    bool variable_code_lengths;
    size_t fixed_code_length;
} CodeTable;

// Public methods

CodeTable * CodeTable_Create(size_t code_length);
void CodeTable_Free(CodeTable * code_table);
bool CodeTable_AddEntry(CodeTable * code_table, size_t code, size_t code_length_bits, char * data, size_t data_length_bytes);
bool CodeTable_WriteToFile(CodeTable * code_table, FILE * file);
CodeTable * CodeTable_ReadFromFile(FILE * file);

// Private methods

void _CodeTable_FreeEntry(_CodeTableEntry * entry);
void _CodeTable_WriteEntryToFile(_CodeTableEntry * entry, FILE * file);
bool _CodeTable_ReadEntryFromFile(_CodeTableEntry * entry, FILE * file);

#endif