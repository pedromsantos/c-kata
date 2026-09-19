#include "checksum_calculator.h"

#include <stdio.h>
#include <string.h>

/* Connascence of Algorithm: this loop (sum of byte values mod 10) is
 * duplicated in checksum_calculator_check() below instead of being
 * extracted into one shared helper -- that duplication is the violation. */
char* checksum_calculator_add_checksum(const char* input_data, char* out_buffer, size_t out_buffer_size) {
    int sum = 0;
    for (size_t i = 0; i < strlen(input_data); i++) {
        sum += (unsigned char)input_data[i];
    }
    int checksum = sum % 10;
    snprintf(out_buffer, out_buffer_size, "%s%d", input_data, checksum);
    return out_buffer;
}

/* Same algorithm as above, independently reimplemented. */
int checksum_calculator_check(const char* input_data_with_checksum) {
    size_t length = strlen(input_data_with_checksum);
    if (length == 0) {
        return 0;
    }

    char checksum_digit = input_data_with_checksum[length - 1];
    int sum = 0;
    for (size_t i = 0; i < length - 1; i++) {
        sum += (unsigned char)input_data_with_checksum[i];
    }
    int expected_checksum = sum % 10;

    return (checksum_digit - '0') == expected_checksum;
}
