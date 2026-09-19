# Code Smells Kata - TicTacToe Refactoring

## Overview

This kata contains a deliberately "smelly" implementation of TicTacToe that needs refactoring. Your goal is to identify and fix various code smells while maintaining functionality.

## Layout: a cross-file verification fixture

The implementation is split across `board.h`/`board.c` (the `Tile`/`Board` data types), `row_winner_checker.h`/`.c`, `column_winner_checker.h`/`.c`, and `diagonal_winner_checker.h`/`.c` (one line-checking strategy per file), and `game.h`/`game.c` (`Game`, which wires the three checkers together in `game_determine_winner`). This mirrors the same split done in the TypeScript reference kata (`15_SmellyTicTacToe`), and exists to give static-analysis/AI code-review tooling (specifically [jev-review](https://github.com/pedromsantos/jev-review)) a genuinely multi-file example of cross-file code smells to check its rules against:

- **Duplicated Code**: `row_winner_checker.c`, `column_winner_checker.c`, and `diagonal_winner_checker.c` each re-implement the same "are these three tiles non-empty and equal" pattern independently rather than sharing a helper -- deliberately, not an oversight.
- **Shotgun Surgery**: adding a fourth line-checking strategy means touching several files (a new checker pair, plus `game.c`'s wiring).
- **Divergent Change**: `game.c` is the file that changes for multiple unrelated reasons -- move-validation rule changes, or wiring in a new line-checking strategy.

Originally `Winner()`/`game_determine_winner()` only ever checked rows, never columns or diagonals -- a real, pre-existing gap in this kata. The column and diagonal checkers close that gap as part of the split, rather than being purely cosmetic additions.

This kata also now has a real test suite (`test_kata.c`, 14 cmocka tests) for the first time -- previously there was no test file registered in the build at all.

## Code Smells to Look For

The implementation contains the following code smells:

1. **Primitive Obsession**
   - Using primitive types where structs would be more appropriate

2. **Feature Envy**
   - A function accessing data from another struct more than its own

3. **Data Struct**
   - Structs with only data and no behavior (acceptable in C, but consider grouping related functions)

4. **Message Chain**
   - Long chains of function calls (Law of Demeter violations)

5. **Long Function**
   - Functions that are too long and do multiple things

6. **Comments**
   - Excessive or unnecessary comments that could be replaced with clearer code

7. **Long Parameter List**
   - Functions with too many parameters

8. **Shotgun Surgery**
   - Changes requiring multiple small edits across many files

9. **Duplicated Code**
   - Similar code appearing in multiple places

10. **Large File**
    - Files that have too many responsibilities

11. **Divergent Change**
    - File being changed for multiple different reasons

12. **Data Clump**
    - Groups of data items that always appear together

13. **Dead Code**
    - Unused code that should be removed

## Tasks

1. Review the code and identify all code smells
2. Add comments marking each code smell you find
3. Refactor the code using small, incremental steps
4. Ensure all tests remain passing after each refactoring

## Tips

- Make one change at a time
- Run tests after each change
- Follow the boy scout rule: leave the code better than you found it

## Example Refactorings in C

### Before: Primitive Obsession

```c
void game_play(int x, int y, char player);
```

### After: Using Structs

```c
typedef struct {
    int x;
    int y;
} Position;

void game_play(Position pos, char player);
```

### Before: Long Function

```c
char determine_winner(Game* game) {
    // 50+ lines of nested conditionals
}
```

### After: Extract Functions

```c
static bool is_row_win(Game* game, int row);
static bool is_col_win(Game* game, int col);
static bool is_diag_win(Game* game);

char determine_winner(Game* game) {
    // Calls helper functions
}
```

## Resources

- [Code Smells Video Tutorial](https://www.youtube.com/watch?v=MM6_tyvBRXE)
- [Refactoring Guru - Code Smells](https://refactoring.guru/refactoring/smells)
- [Comprehensive Code Smells Guide](https://luzkan.github.io/smells/)
