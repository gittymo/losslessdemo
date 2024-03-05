/*  libcodetable.c */

#include "libcodetable.h"

CodeTable * CodeTable_Create(size_t code_length)
{
    if (code_length > 64) return NULL;

    CodeTable * code_table = (CodeTable *) malloc(sizeof(CodeTable));
    if (!code_table) return NULL;

    code_table->entries = (_CodeTableEntry **) malloc(sizeof(_CodeTableEntry *) * LIBCODETABLE_ENTRIES_BLOCK_SIZE);
    if (!code_table->entries) {
        free(code_table);
        return NULL;
    } else {
        code_table->entries_array_size = LIBCODETABLE_ENTRIES_BLOCK_SIZE;
        code_table->entries_count = 0;
        code_table->variable_code_lengths = code_length == 0;
        code_table->fixed_code_length = code_length;
    }

    return code_table;
}

void CodeTable_Free(CodeTable * code_table)
{
    if (!code_table) return;
    if (code_table->entries) {
        for (size_t e = 0; e < code_table->entries_count; e++) {
            if (code_table->entries[e]) {
                _CodeTable_FreeEntry(code_table->entries[e]);
                code_table->entries[e] = NULL;
            }
        }
        free(code_table->entries);
    }
    code_table->entries = NULL;
    code_table->entries_array_size = code_table->entries_count = 0;
    code_table->fixed_code_length = 0;
    code_table->variable_code_lengths = false;
    free(code_table);
}

bool CodeTable_AddEntry(CodeTable * code_table, size_t code, size_t code_length_bits, char * data, size_t data_length_bytes)
{
    if (!code_table || !data || code_length_bits == 0 || data_length_bytes == 0) return false;

    bool found = false;
    for (size_t e = 0; e < code_table->entries_count && !found; e++) {
        if (code == code_table->entries[e]->code && code_length_bits == code_table->entries[e]->code_length_bits) {
            found = true;
            code_table->entries[e]->data = data;
            code_table->entries[e]->data_length_bytes = data_length_bytes;
        }
    }
    if (!found) {
        _CodeTableEntry * new_entry = (_CodeTableEntry *) malloc(sizeof(_CodeTableEntry));
        if (!new_entry) return false;
        new_entry->code = code;
        new_entry->code_length_bits = code_length_bits;
        new_entry->data = data;
        new_entry->data_length_bytes = data_length_bytes;

        if (code_table->entries_count == code_table->entries_array_size) {
            _CodeTableEntry ** new_entries_array = 
                (_CodeTableEntry **) realloc(code_table->entries, sizeof(_CodeTableEntry *) * (code_table->entries_count + LIBCODETABLE_ENTRIES_BLOCK_SIZE));
            if (!new_entries_array) return false;
            code_table->entries = new_entries_array;
            code_table->entries_array_size += LIBCODETABLE_ENTRIES_BLOCK_SIZE;
        }

        code_table->entries[code_table->entries_count++] = new_entry;
    }
    return true;
}

void _CodeTable_FreeEntry(_CodeTableEntry * entry)
{
    if (!entry) return;
    entry->code = 0;
    entry->code_length_bits = 0;
    if (entry->data) free(entry->data);
    entry->data = NULL;
    entry->data_length_bytes = 0;
    free(entry);
}

void _CodeTable_WriteEntryToFile(_CodeTableEntry * entry, FILE * file)
{
    if (!entry || !file) return;
    fwrite(&entry->code_length_bits, 1, sizeof(u_char), file);
    if (entry->code_length_bits <= 8) fwrite(&entry->code, 1, sizeof(u_char), file);
    else if (entry->code_length_bits <= 16) fwrite(&entry->code, 1, sizeof(u_int16_t), file);
    else if (entry->code_length_bits <= 32) fwrite(&entry->code, 1, sizeof(u_int32_t), file);
    else fwrite(&entry->code, 1, sizeof(u_int64_t), file);
    fwrite(&entry->data_length_bytes, 1, sizeof(uint32_t), file);
    fwrite(&entry->data, entry->data_length_bytes, sizeof(u_char), file);
}

bool CodeTable_WriteToFile(CodeTable * code_table, FILE * file)
{
    if (!code_table || !file) return false;
    // Write the boolean flag indicating if the table uses fixed length codes.
    fwrite(&code_table->variable_code_lengths, 1, sizeof(bool), file);
    // Write the fixed code length.
    fwrite(&code_table->fixed_code_length, 1, sizeof(u_char), file);
    // Write the number of code entries.
    fwrite(&code_table->entries_count, 1, sizeof(int32_t), file);
    // Write the code entries.
    for (size_t e = 0; e < code_table->entries_count; e++) {
        _CodeTable_WriteEntryToFile(code_table->entries[e], file);
    }
    return true;
} 

CodeTable * CodeTable_ReadFromFile(FILE * file)
{
    if (!file) return NULL;
    CodeTable * code_table = (CodeTable *) malloc(sizeof(CodeTable));
    if (!code_table) return NULL;
    fread(&code_table->variable_code_lengths, 1, sizeof(bool), file);
    fread(&code_table->fixed_code_length, 1, sizeof(u_char), file);
    fread(&code_table->entries_count, 1, sizeof(u_int32_t), file);
    code_table->entries = (_CodeTableEntry **) malloc(sizeof(_CodeTableEntry) * code_table->entries_count);
    if (!code_table->entries) {
        CodeTable_Free(code_table);
        return NULL;
    }
    code_table->entries_array_size = code_table->entries_count;
    for (size_t e = 0; e < code_table->entries_count; e++) {
        _CodeTableEntry * new_entry = (_CodeTableEntry *) malloc(sizeof(_CodeTableEntry));
        if (!new_entry) {
            CodeTable_Free(code_table);
            return NULL;
        }
        if (!_CodeTable_ReadEntryFromFile(new_entry, file)) {
            CodeTable_Free(code_table);
            return NULL;
        }
        code_table->entries[e] = new_entry;
    }
    return code_table;
}

bool _CodeTable_ReadEntryFromFile(_CodeTableEntry * entry, FILE * file)
{
    if (!entry || !file) return false;
    fread(&entry->code_length_bits, 1, sizeof(u_char), file);
    if (entry->code_length_bits <= 8) fread(&entry->code, 1, sizeof(u_char), file);
    else if (entry->code_length_bits <= 16) fread(&entry->code, 1, sizeof(u_int16_t), file);
    else if (entry->code_length_bits <= 32) fread(&entry->code, 1, sizeof(u_int32_t), file);
    else fread(&entry->code, 1, sizeof(u_int64_t), file);
    fread(&entry->data_length_bytes, 1, sizeof(u_int32_t), file);
    entry->data = (char *) malloc(entry->data_length_bytes);
    if (!entry->data) return false;
    fread(entry->data, 1, entry->data_length_bytes, file);
    return true;
}