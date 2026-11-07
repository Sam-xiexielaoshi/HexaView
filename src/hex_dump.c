#include "hex_dump.h"

#include <ctype.h>
#include <inttypes.h>
#include <errno.h>

int hex_dump_print(FILE* file, uintmax_t file_size) {
	if(file == NULL) {
		errno=EINVAL;
		return -1;
	}

	unsigned char buffer[HEX_DUMP_BYTES_PER_ROW];
	uintmax_t offset= 0;
	uintmax_t remaining = file_size;
	size_t bytes_in_row;

	while(remaining > 0) {
		size_t bytes_to_read = remaining < sizeof(buffer) ? (size_t)remaining : sizeof(buffer);
		bytes_in_row = fread(buffer, 1, bytes_to_read, file);
		if(bytes_in_row == 0) {
			if(ferror(file)) return -1;
			errno = EIO;
			return -1;
		}

		printf("%08" PRIXMAX "    ", offset);
		for (size_t i = 0; i < HEX_DUMP_BYTES_PER_ROW; i++) {
			if (i < bytes_in_row) {
				printf("%02X ", buffer[i]);
			} else {
				printf("   ");
			}
		}
		printf(" ");

		for (size_t i = 0; i < bytes_in_row; i++) {
			unsigned char byte = buffer[i];
			if (isprint(byte)) {
				printf("%c", byte);
			} else {
				printf(".");
			}
		}
		printf("\n");
		offset += (uintmax_t)bytes_in_row;
		remaining -= (uintmax_t)bytes_in_row;
	}
	return 0;
}

