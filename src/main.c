#include <stdio.h>
#include <inttypes.h>
#include "file_reader.h"

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

	printf("nHexaView - Binary File Inspector\n");
	printf("--------------------------------\n");

    printf("File size:  %ld bytes\n", info.file_size);
    printf("Bytes read: %" PRIuMAX "\n", info.bytes_read);
    printf("Chunks:     %" PRIuMAX "\n", info.chunks_read);
	return 0;
}
