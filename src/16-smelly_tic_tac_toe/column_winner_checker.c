#include "column_winner_checker.h"

char column_winner_checker_check(Board* board) {
    // if the positions in first column are taken
    if (board_tile_at(board, 0, 0)->symbol != ' ' &&
        board_tile_at(board, 1, 0)->symbol != ' ' &&
        board_tile_at(board, 2, 0)->symbol != ' ') {
        // if first column is full with same symbol
        if (board_tile_at(board, 0, 0)->symbol == board_tile_at(board, 1, 0)->symbol &&
            board_tile_at(board, 2, 0)->symbol == board_tile_at(board, 1, 0)->symbol) {
            return board_tile_at(board, 0, 0)->symbol;
        }
    }

    // if the positions in middle column are taken
    if (board_tile_at(board, 0, 1)->symbol != ' ' &&
        board_tile_at(board, 1, 1)->symbol != ' ' &&
        board_tile_at(board, 2, 1)->symbol != ' ') {
        // if middle column is full with same symbol
        if (board_tile_at(board, 0, 1)->symbol == board_tile_at(board, 1, 1)->symbol &&
            board_tile_at(board, 2, 1)->symbol == board_tile_at(board, 1, 1)->symbol) {
            return board_tile_at(board, 0, 1)->symbol;
        }
    }

    // if the positions in last column are taken
    if (board_tile_at(board, 0, 2)->symbol != ' ' &&
        board_tile_at(board, 1, 2)->symbol != ' ' &&
        board_tile_at(board, 2, 2)->symbol != ' ') {
        // if last column is full with same symbol
        if (board_tile_at(board, 0, 2)->symbol == board_tile_at(board, 1, 2)->symbol &&
            board_tile_at(board, 2, 2)->symbol == board_tile_at(board, 1, 2)->symbol) {
            return board_tile_at(board, 0, 2)->symbol;
        }
    }

    return ' ';
}
