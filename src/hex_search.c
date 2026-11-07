#include "hex_search.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>

static int hex_search_add_result(HexSearchResults* results, uintmax_t offset) {
	if(results->count == results->capacity) {
		size_t new_capacity;
		if(results->capacity == 0) new_capacity = 8;
		else {
			if(results->capacity > SIZE_MAX / 2) {
				errno = ENOMEM;
				return -1;
			}
			new_capacity = results->capacity * 2;
		}
		uintmax_t* new_offsets = realloc(results->offsets, new_capacity * sizeof(uintmax_t));
		if(new_offsets == NULL) return -1;
		results->offsets = new_offsets;
		results->capacity = new_capacity;
	}
	results->offsets[results->count] = offset;
	results->count++;
	return 0;
}

void hex_search_free(HexSearchResults* results) {
	if(results==NULL) return;
	free(results->offsets);
	results->offsets = NULL;
	results->count = 0;
	results->capacity = 0;
}

int hex_search_find(FILE* file, const unsigned char* pattern, size_t pattern_size, HexSearchResults* results) {
	if(file == NULL || pattern == NULL || pattern_size == 0 || results == NULL) {
		errno = EINVAL;
		return -1;
	}
	if(pattern_size == SIZE_MAX){
		errno=ENOMEM;
		return -1;
	}
	unsigned char* buffer = malloc(pattern_size);
	if(buffer == NULL) return -1;
	if(fseek(file, 0, SEEK_SET) != 0) {
		int saved_errno =errno;
		free(buffer);
		errno=saved_errno;
		return -1;
	}
	size_t bytes_read = fread(buffer, 1, pattern_size, file);
	if(bytes_read < pattern_size) {
		int read_failed = ferror(file);
		free(buffer);
		if(read_failed) return -1;
		return 0;
	}
	int status = 0;
	uintmax_t offset = 0;

	while(1){
		if(memcmp(buffer, pattern, pattern_size) == 0) {
			if(hex_search_add_result(results, offset) !=0) {
				status = -1;
				break;
			}
		}
		int next_byte = fgetc(file);
		if(next_byte == EOF) {
			if(ferror(file)) status = -1;
			break;
		}
		memmove(buffer,buffer+1, pattern_size -1);
		buffer[pattern_size - 1] = (unsigned char)next_byte;
		offset++;
	}
	int saved_errno =errno;
	free(buffer);
	errno =saved_errno;
	return status;
}
