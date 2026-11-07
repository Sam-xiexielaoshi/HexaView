#ifndef HEX_SEARCH_H
#define HEX_SEARCH_H
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
	uintmax_t* offsets;
	size_t count;
	size_t capacity;
} HexSearchResults;

int hex_search_find(FILE* file, const unsigned char* pattern, size_t pattern_size, HexSearchResults* results);
void hex_search_free(HexSearchResults* results);

#endif
