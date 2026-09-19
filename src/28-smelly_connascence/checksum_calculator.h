#pragma once

/*
 * Connascence Kata - Connascence of Algorithm, C port of
 * ChecksumCalculator.ts.
 *
 * Algorithm violation: addChecksum and check both independently compute
 * sum(charCode) % 10 -- the two functions must agree on the same algorithm
 * without sharing a single implementation of it.
 */

#include <stddef.h>

char* checksum_calculator_add_checksum(const char* input_data, char* out_buffer, size_t out_buffer_size);
int checksum_calculator_check(const char* input_data_with_checksum);
