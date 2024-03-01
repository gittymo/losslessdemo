/*  rledemo.c 
    Demonstration of Runlength Encoding for Warrick
    (C)2024 Morgan Evans */

#include <unistd.h>

#include "librle.h"

int main(int argc, char * argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Usage: %s -c|-d <source file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int compress = -1;
    int c;

    while ((c = getopt(argc, argv, "cd")) != -1) {
        switch (c) {
            case 'c' : compress = 1; break;
            case 'd' : compress = 0; break;
        }
    }

    if (compress < 0) {
        fprintf(stderr, "Please use either the -c (compress) or -d (decompress) command line arguments.\n");
        exit(EXIT_FAILURE);
    }

    FILE * in_file = fopen(argv[2], "r");
    if (!in_file) {
        fprintf(stderr, "Failed to open source file '%s'.\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    struct stat st;
    stat(argv[2], &st);
    char * source_data = (char *) malloc(st.st_size);
    size_t source_data_size = fread(source_data, 1, st.st_size, in_file);
    fclose(in_file);

    size_t out_data_size;
    unsigned char * out_data;

    if (compress) {
        out_data = RLE_Compress(source_data, st.st_size, &out_data_size);
    } else {
        out_data = RLE_Decompress(source_data, st.st_size, &out_data_size);
    }
    
    FILE * out_file = fopen(argv[3],"wb");
    if (!out_file) {
        fprintf(stderr, "Failed to open output file '%s' for writing.\n", argv[3]);
    } else {
        fwrite(out_data, 1, out_data_size, out_file);
        fclose(out_file);
    }

    free(source_data);
    free(out_data);

    exit(EXIT_SUCCESS);
}