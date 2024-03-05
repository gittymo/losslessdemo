/*  libfile.c */

#include "libfile.h"

unsigned char * loadTextFile(const char * FILENAME, size_t * data_length_ptr)
{
    if (!FILENAME || !data_length_ptr) return NULL;
    *data_length_ptr = 0;

    struct stat st;
    stat(FILENAME, &st);

    if (!S_ISREG(st.st_mode)) return NULL;

    unsigned char * data = (unsigned char *) malloc(st.st_size);
    if (!data) return NULL;

    FILE * infile = fopen(FILENAME,"r");
    if (!infile) return NULL;

    fread(data, 1, st.st_size, infile);
    fclose(infile);

    *data_length_ptr = st.st_size;
    return data;
}

unsigned char * loadDataFile(const char * FILENAME, size_t * data_length_ptr)
{
    if (!FILENAME || !data_length_ptr) return NULL;
    *data_length_ptr = 0;

    struct stat st;
    stat(FILENAME, &st);

    if (!S_ISREG(st.st_mode)) return NULL;

    unsigned char * data = (unsigned char *) malloc(st.st_size);
    if (!data) return NULL;

    FILE * infile = fopen(FILENAME,"rb");
    if (!infile) return NULL;

    fread(data, 1, st.st_size, infile);
    fclose(infile);

    *data_length_ptr = st.st_size;
    return data;
}

void saveDataFile(const char * FILENAME, const unsigned char * DATA, const size_t DATA_LENGTH)
{
    if (!FILENAME || !DATA || DATA_LENGTH == 0) return;

    FILE * outfile = fopen(FILENAME,"wb");
    fwrite(DATA, 1, DATA_LENGTH, outfile);
    fclose(outfile);
}

void saveTextFile(const char * FILENAME, const unsigned char * DATA, const size_t DATA_LENGTH)
{
    if (!FILENAME || !DATA || DATA_LENGTH == 0) return;

    FILE * outfile = fopen(FILENAME,"w");
    fwrite(DATA, 1, DATA_LENGTH, outfile);
    fclose(outfile);
}

unsigned char * createFilepathWithNewExtension(char * filepath, char * newRootDirPath, char * new_extension)
{
    // Make sure the calling method has provided sensible parameter values.  If not return no data.
    if (!filepath || filepath[0] == 0 || !new_extension || new_extension[0] == 0) return NULL;

    // Get the base filename pointed to by 'filepath'.
    char * bname = basename(filepath);
    size_t ext_index = strchr(bname, '.');
    if (ext_index > 0) bname[ext_index] = 0;

    // Get the root directory path to store the new file in, either from 'filepath' or from the string given
    // by the parameter value in 'newRootDirPath'.
    char * dname = newRootDirPath && newRootDirPath[0] != 0 ? newRootDirPath : dirname(filepath);

    // Create a buffer to hold the newly created filepath.
    char * new_filepath = (char *) malloc(LIBFILE_MAX_FILEPATH_LENGTH);

    // If we can't create the buffer, exit returning no data.
    if (!new_filepath) return NULL;
    // Copy the required root folder path, filename and extension data into the new_filepath buffer.
    size_t new_filepath_length = snprintf(new_filepath, LIBFILE_MAX_FILEPATH_LENGTH, "%s/%s.%s", dname, bname, new_extension);

    // Return a pointer to an optimally resized buffer giving the newly created filepath.
    return realloc(new_filepath, new_filepath_length);
}