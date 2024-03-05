/*  rledemo.c 
    Demonstration of Runlength Encoding for Warrick
    (C)2024 Morgan Evans */

#include <unistd.h>

#include "librle.h"
#include "libfile.h"

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

    size_t source_data_size;
    unsigned char * source_data = loadDataFile(argv[2], &source_data_size);

    size_t out_data_size;
    unsigned char * out_data;

    if (compress) {
        out_data = RLE_Compress(source_data, source_data_size, &out_data_size);
    } else {
        out_data = RLE_Decompress(source_data, source_data_size, &out_data_size);
    }
    
    saveDataFile(argv[3], out_data, out_data_size);

    free(source_data);
    free(out_data);

    exit(EXIT_SUCCESS);
}