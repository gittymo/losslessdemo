/*   huffdemo.c */

#include "libhuffman.h"
#include "libfile.h"

int main(int argc, char * argv[])
{
    // Make sure the user has passed a path to a file to compress as a command line argument.
    // If not, return a message showing how to correctly use the demo.
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Load the source data or exit if we can't.
    size_t source_data_length;
    unsigned char * source_data = loadDataFile(argv[1], &source_data_length);
    if (!source_data) {
        fprintf(stderr,"Failed to load source data from file '%s'.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Encode the source data.
    size_t encoded_data_length;
    unsigned char * encoded_data = Huffman_Encode(source_data, source_data_length, &encoded_data_length);

    // If we got encoded data, save it as a .huf file.
    if (encoded_data) {
        char * output_filepath = createFilepathWithNewExtension(argv[1], NULL, ".huf");
        saveDataFile(output_filepath, encoded_data, encoded_data_length);
        free(output_filepath);
    }

    // Don't forget to clean up any malloc'd data.
    free(source_data);
    exit(EXIT_SUCCESS);
}