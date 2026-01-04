#include "kata.h"
#include <stdlib.h>

Cell cell_create(Row row, Column column) {
    Cell cell;
    cell.row = row;
    cell.column = column;
    return cell;
}

bool cell_equals(Cell* a, Cell* b) {
    return a->row == b->row && a->column == b->column;
}

Turn turn_create(Cell cell, Player player) {
    Turn turn;
    turn.cell = cell;
    turn.player = player;
    return turn;
}

bool turn_equals(Turn* a, Turn* b) {
    return a->player == b->player && cell_equals(&a->cell, &b->cell);
}

TicTacToe* tic_tac_toe_create(void) {
    TicTacToe* game = (TicTacToe*)malloc(sizeof(TicTacToe));
    if (game) {
        game->data = NULL;
    }
    return game;
}

void tic_tac_toe_destroy(TicTacToe* game) {
    if (game) {
        free(game);
    }
}

void tic_tac_toe_play(TicTacToe* game, Turn turn) {
    (void)game;
    (void)turn;
}

Output* output_create(void) {
    Output* output = (Output*)malloc(sizeof(Output));
    if (output) {
        output->data = NULL;
    }
    return output;
}

void output_destroy(Output* output) {
    if (output) {
        free(output);
    }
}

void output_print_play(Output* output, int x, int y, const char* player) {
    (void)output;
    (void)x;
    (void)y;
    (void)player;
}

void output_print_winner(Output* output, const char* player) {
    (void)output;
    (void)player;
}

void output_print_error(Output* output, const char* error_message) {
    (void)output;
    (void)error_message;
}
