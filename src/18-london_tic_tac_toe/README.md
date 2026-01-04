# Tic Tac Toe kata

## Rules

- X always plays first
- Players alternate placing X's and O's on the board
- Players cannot play on a played position
- A Player wins when it has three in a row
  - Horizontally
  - Vertically
  - Diagonally
- If all nine squares are filled and neither player has won, the game is a draw

In this version of TicTacToe nothing is returned but a call to an Output is made to print the game events.

## Interface

Use the type definitions in `kata.h` to get started.

```c
typedef enum {
    PLAYER_X = 1,
    PLAYER_O = 2
} Player;

typedef enum {
    TOP = 1,
    MIDDLE = 2,
    BOTTOM = 3
} Row;

typedef enum {
    LEFT = 1,
    CENTER = 2,
    RIGHT = 3
} Column;

typedef struct {
    Row row;
    Column column;
} Cell;

typedef struct {
    Cell cell;
    Player player;
} Turn;
```

### Example Usage

```c
#include "kata.h"

int main(void) {
    TicTacToe* game = tic_tac_toe_create();
    Output* output = output_create();

    // Player X makes first move
    Cell cell = cell_create(TOP, LEFT);
    Turn turn = turn_create(cell, PLAYER_X);
    tic_tac_toe_play(game, turn);

    // Player O makes second move
    cell = cell_create(MIDDLE, CENTER);
    turn = turn_create(cell, PLAYER_O);
    tic_tac_toe_play(game, turn);

    tic_tac_toe_destroy(game);
    output_destroy(output);
    return 0;
}
```
