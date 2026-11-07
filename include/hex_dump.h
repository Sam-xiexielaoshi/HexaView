#ifndef HEX_DUMP_H
#define HEX_DUMP_H

#include <stdio.h>
#include <stdint.h>

#define HEX_DUMP_BYTES_PER_ROW 16

int hex_dump_print(FILE* file, uintmax_t file_size);

#endif
