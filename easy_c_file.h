// easy_c_file.h

#ifndef EASY_C_FILE_H
#define EASY_C_FILE_H
#include <stddef.h>

int createDirectory(const char* path);
int writeFile(const char* filename, const char* data);
int readFile(const char* filename, char** data);
int read_binary_file(const char *filename, void **data, size_t *size);
int write_binary_file(const char *filename, const void *data, size_t size);
int appendLine(const char* filename, const char* line);
int deleteFile(const char* filename);
int listFilesInDirectory(const char* directory, char*** fileList, int* numFiles);
int deleteDirectory(const char* directory);

#endif
