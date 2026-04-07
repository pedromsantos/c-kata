# Code Smells Kata - TicTacToe Refactoring

## Overview

This kata contains a deliberately "smelly" implementation of TicTacToe that needs refactoring. Your goal is to identify and fix various code smells while maintaining functionality.

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
