# Tennis kata

## Source

<https://github.com/emilybache/Tennis-Refactoring-Kata>

## Problem Statement

You are taking over a tennis scoring system project from a colleague who:

- Has spent 8.5 hours of a 10-hour billable project
- Claims the work is complete with passing tests
- Is currently unavailable due to illness

Your manager has requested you to:

1. Spend the remaining billable time (~1 hour) improving the code
2. Prepare feedback on the current design
3. Be ready to discuss the value of refactoring beyond billable hours

## Your task

Refactor the code to improve its readability.

### Guidelines

#### Before you start

- Make sure the tests have a good coverage
  - Use a code coverage tool to make sure code coverage by tests is very high
  - For this exercise code coverage is high (100%)

##### Execute tests

```sh
make test_tennis
```

##### Execute tests with coverage (using gcov/lcov)

```sh
gcc -fprofile-arcs -ftest-coverage src/tennis/kata.c src/tennis/test_kata.c -o src/tennis/test_kata -I.
./src/tennis/test_kata
gcov kata.c
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_html
```

#### While refactoring

- Stay in the green while refactoring
  - Run the tests after each refactor
    - Check all tests still pass
    - Check code coverage has not dropped
- Commit after each refactor
- In case of persistent test fails, use `git reset` to go back to green

#### Improve readability

1. Tackle clutter by
   - Formatting the code consistently
     - Don't force the reader to waste time due to inconsistent formatting
   - Renaming bad names or abbreviations on variables, arguments, parameters, functions, and types
     - <https://www.digdeeproots.com/articles/naming-process/>
2. Tackle Comments and Dead Code by
   - Deleting useless comments
   - Deleting useful comments by extracting a function named after them
   - Deleting dead code
     - Don't make the reader waste time figuring out code that is not used
3. Tackle implicit knowledge by
   - Extracting constants from magic numbers and strings
   - Extracting complex conditional expressions
4. Tackle scattering by
   - Refining the scope for variables
   - Ensuring variables are declared close to where they are used
   - Grouping public functions at the top of the file to show first what matters the most

#### Reduce complexity

1. Tackle complexity by
   - Extracting smaller static functions from long functions
     - Also encapsulate any cryptic code (that cannot be made more explicit) in static functions
   - Extracting static functions from nested code blocks
   - Returning from functions as soon as possible
2. Tackle duplication by
   - Removing duplicated knowledge

### Example Usage

```c
#include "kata.h"
#include <stdio.h>

int main(void) {
    TennisGame1* game = tennis_game1_create("player1", "player2");

    tennis_game1_won_point(game, "player1");
    tennis_game1_won_point(game, "player2");
    tennis_game1_won_point(game, "player1");

    char* score = tennis_game1_score(game);
    printf("Score: %s\n", score);  // "Thirty-Fifteen"

    tennis_game1_destroy(game);
    return 0;
}
```
