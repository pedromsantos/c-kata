# Prime Factors Kata

Write a function that decomposes a positive integer into its prime factors.

## Problem Description

Given a positive integer, return an array of its prime factors in ascending order. A prime factor is a prime number that divides the input number without leaving a remainder.

## Function Signature

```c
// Returns an array of prime factors and sets the count
int* prime_factors(int number, int* count);
```

## Examples

| **Input** | **Output**  | **Explanation** |
| --------- | ----------- | --------------- |
| 2         | `[2]`       | 2 is prime      |
| 3         | `[3]`       | 3 is prime      |
| 4         | `[2, 2]`    | 4 = 2 × 2       |
| 6         | `[2, 3]`    | 6 = 2 × 3       |
| 9         | `[3, 3]`    | 9 = 3 × 3       |
| 12        | `[2, 2, 3]` | 12 = 2 × 2 × 3  |
| 15        | `[3, 5]`    | 15 = 3 × 5      |

### Example Usage

```c
#include "kata.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int count;
    int* factors = prime_factors(12, &count);

    printf("Prime factors of 12: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", factors[i]);
    }
    printf("\n");  // Output: 2 2 3

    free(factors);
    return 0;
}
```

## TDD Rules

Follow these Test-Driven Development rules strictly:

1. ✅ Write production code only to pass a failing unit test
2. ✅ Write only enough of a unit test to make it fail
3. ✅ Write only enough production code to make the failing test pass

## Getting Started

1. Create a function that takes a positive integer as input
2. Return an array of prime factors
3. Follow the TDD cycle: Red → Green → Refactor

## Edge Cases to Consider

- Input of 1 should return an empty array
- Input must be a positive integer
- Large numbers should still work efficiently

Source: [Cyber-Dojo](http://cyber-dojo.org/)
