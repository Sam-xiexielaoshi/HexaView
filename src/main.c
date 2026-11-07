#include <stdio.h>
#include <inttypes.h>
#include "file_reader.h"
#include "hex_dump.h"

int main(int argc, char* argv[]) {
	if(argc !=2) {
		printf("Usage: %s <file_path>\n", argv[0]);
		return 1;
	}

	FileInfo info;

	if (file_reader_inspect(argv[1], &info) != 0) {
		perror("HexaView");
		return 1;
	}

	printf("\nHexaView - Binary File Inspector\n");
	printf("--------------------------------\n");

        printf("File size:  %ld bytes\n", info.file_size);
        printf("Bytes read: %" PRIuMAX "\n", info.bytes_read);
        printf("Chunks:     %" PRIuMAX "\n", info.chunks_read);

	printf("\nHexadecimal Dump\n");
	printf("-------------------------------\n");
	FILE* file = fopen(argv[1], "rb");
	if(file==NULL) {
		perror("HexaView");
		return 1;
	}

	if(hex_dump_print(file, (uintmax_t)info.file_size) != 0){
		perror("Hex Dump");
		fclose(file);
		return 1;
	}

	if(fclose(file) != 0) {
		perror("HexaView");
		return 1;
	}

	return 0;

}
