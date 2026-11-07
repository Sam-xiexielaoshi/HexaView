#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdint.h>

typedef struct{
long file_size;
uintmax_t bytes_read;
uintmax_t chunks_read;
} FileInfo;

int file_reader_inspect(const char* file_path, FileInfo* info);

#endif
