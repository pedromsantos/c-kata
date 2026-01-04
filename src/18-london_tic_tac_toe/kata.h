#ifndef LONDON_TIC_TAC_TOE_H
#define LONDON_TIC_TAC_TOE_H

#include <stdbool.h>

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

typedef struct {
    void* data;
} TicTacToe;

typedef struct {
    void* data;
} Output;

Cell cell_create(Row row, Column column);
bool cell_equals(Cell* a, Cell* b);

Turn turn_create(Cell cell, Player player);
bool turn_equals(Turn* a, Turn* b);

TicTacToe* tic_tac_toe_create(void);
void tic_tac_toe_destroy(TicTacToe* game);
void tic_tac_toe_play(TicTacToe* game, Turn turn);

Output* output_create(void);
void output_destroy(Output* output);
void output_print_play(Output* output, int x, int y, const char* player);
void output_print_winner(Output* output, const char* player);
void output_print_error(Output* output, const char* error_message);

#endif /* LONDON_TIC_TAC_TOE_H */
