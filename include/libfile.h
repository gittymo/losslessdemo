/*  libfile.h */

#ifndef COM_PLUS_MEVANSPN_LIBFILE
#define COM_PLUS_MEVANSPN_LIBFILE

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>

#define LIBFILE_MAX_FILEPATH_LENGTH 4096

unsigned char * loadTextFile(const char * FILENAME, size_t * data_length_ptr);
unsigned char * loadDataFile(const char * FILENAME, size_t * data_length_ptr);

void saveTextFile(const char * FILENAME, const unsigned char * DATA, const size_t DATA_LENGTH);
void saveDataFile(const char * FILENAME, const unsigned char * DATA, const size_t DATA_LENGTH);

unsigned char * createFilepathWithNewExtension(char * filepath, char * newRootDirPath, char * new_extension);

#endif