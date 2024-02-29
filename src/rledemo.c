/*  rledemo.c 
    Demonstration of Runlength Encoding for Warrick
    (C)2024 Morgan Evans */

#include "librle.h"

int main(int argc, char * argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <source file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Read the input file.
    FILE * in_file = fopen(argv[1], "r");
    if (!in_file) {
        fprintf(stderr, "Failed to open source file '%s'.\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    struct stat st;
    stat(argv[1], &st);
    char * source_data = (char *) malloc(st.st_size);
    size_t source_data_size = fread(source_data, 1, st.st_size, in_file);
    fclose(in_file);

    // Get the compressed data.
    size_t compressed_data_size;
    char * compressed_data = RLE_Compress(source_data, st.st_size, &compressed_data_size);

    FILE * out_file = fopen(argv[2],"wb");
    if (!out_file) {
        fprintf(stderr, "Failed to open output file '%s' for writing.\n", argv[2]);
    } else {
        fwrite(compressed_data, 1, compressed_data_size, out_file);
        fclose(out_file);
    }

    printf("Compressed %lu bytes to %lu bytes.\n", source_data_size, compressed_data_size);

    free(source_data);
    free(compressed_data);
    exit(EXIT_SUCCESS);
}