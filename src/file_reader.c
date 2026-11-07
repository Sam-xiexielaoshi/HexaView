#include "file_reader.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#define BUFFER_SIZE 4096

int file_reader_inspect(const char* file_path, FileInfo* info) {
     if (file_path == NULL || info == NULL) {
         errno = EINVAL;
         return -1;
     }

     FILE* file = fopen(file_path, "rb");

     if (file == NULL) return -1;
     if (fseek(file, 0, SEEK_END) != 0) {
         int saved_errno = errno;
       	 fclose(file);
	 errno = saved_errno;
	 return -1;
     }

     long file_size = ftell(file);

     if (file_size < 0) {
	 int saved_errno = errno;
	 fclose(file);
	 errno = saved_errno;
	 return -1;
     }

	if (fseek(file, 0, SEEK_SET) != 0) {
		int saved_errno = errno;
		fclose(file);
		errno = saved_errno;
		return -1;
	}

	unsigned char buffer[BUFFER_SIZE];
	uintmax_t bytes_read = 0;
	uintmax_t chunks_read = 0;
	size_t bytes_in_chunk;

	while ((bytes_in_chunk = fread(buffer, 1, sizeof(buffer), file)) > 0) {
		bytes_read += (uintmax_t)bytes_in_chunk;
		chunks_read++;
	}

	if (ferror(file)) {
		int saved_errno = errno;
		fclose(file);
		errno = saved_errno;
		return -1;
	}

	if (fclose(file) != 0) return -1;

	info->file_size = file_size;
	info->bytes_read = bytes_read;
	info->chunks_read = chunks_read;

     return 0;
}
