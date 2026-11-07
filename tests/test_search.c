#include "hex_search.h"

#include <stdio.h>
#include <stdlib.h>
static void run_test(
    const char* name,
    const unsigned char* data,
    size_t data_size,
    const unsigned char* pattern,
    size_t pattern_size,
    const uintmax_t* expected_offsets,
    size_t expected_count
) {
    FILE* file = tmpfile();

    if (file == NULL) {
        perror("tmpfile");
        exit(EXIT_FAILURE);
    }

    if (fwrite(data, 1, data_size, file) != data_size) {
        perror("fwrite");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    HexSearchResults results = {0};

    int status = hex_search_find(
        file,
        pattern,
        pattern_size,
        &results
    );

    if (status != 0) {
        perror("hex_search_find");
        hex_search_free(&results);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int passed = results.count == expected_count;

    for (size_t i = 0; passed && i < expected_count; i++) {
        if (results.offsets[i] != expected_offsets[i]) {
            passed = 0;
        }
    }

    printf("[%s] %s\n", passed ? "PASS" : "FAIL", name);

    if (!passed) {
        printf("Expected %zu matches, got %zu\n",
               expected_count, results.count);

        for (size_t i = 0; i < results.count; i++) {
            printf("Actual offset: %ju\n", results.offsets[i]);
        }

        hex_search_free(&results);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    hex_search_free(&results);
    fclose(file);
}
int main(void) {

    // Test 1: Single match
    const unsigned char data1[] = "Hello World";
    const unsigned char pattern1[] = "World";
    const uintmax_t expected1[] = {6};

    run_test(
        "Single match",
        data1, sizeof(data1) - 1,
        pattern1, sizeof(pattern1) - 1,
        expected1, 1
    );

    // Test 2: Multiple matches
    const unsigned char data2[] = "ABC ABC ABC";
    const unsigned char pattern2[] = "ABC";
    const uintmax_t expected2[] = {0, 4, 8};

    run_test(
        "Multiple matches",
        data2, sizeof(data2) - 1,
        pattern2, sizeof(pattern2) - 1,
        expected2, 3
    );

    // Test 3: Overlapping matches
    const unsigned char data3[] = {0xAA, 0xAA, 0xAA};
    const unsigned char pattern3[] = {0xAA, 0xAA};
    const uintmax_t expected3[] = {0, 1};

    run_test(
        "Overlapping matches",
        data3, sizeof(data3),
        pattern3, sizeof(pattern3),
        expected3, 2
    );

    // Test 4: No match
    const unsigned char data4[] = "Hello World";
    const unsigned char pattern4[] = "XYZ";

    run_test(
        "No match",
        data4, sizeof(data4) - 1,
        pattern4, sizeof(pattern4) - 1,
        NULL, 0
    );

    // Test 5: Binary pattern
    const unsigned char data5[] = {
        0x48, 0x65, 0x78, 0x00,
        0x41, 0x42, 0x43, 0xFF,
        0x48, 0x65, 0x78
    };

    const unsigned char pattern5[] = {
        0x48, 0x65, 0x78
    };

    const uintmax_t expected5[] = {0, 8};

    run_test(
        "Binary pattern",
        data5, sizeof(data5),
        pattern5, sizeof(pattern5),
        expected5, 2
    );

    printf("\nAll search tests passed!\n");

    return EXIT_SUCCESS;
}
