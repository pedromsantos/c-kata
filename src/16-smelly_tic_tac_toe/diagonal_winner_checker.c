#include "diagonal_winner_checker.h"

char diagonal_winner_checker_check(Board* board) {
    // if the positions in the top-left to bottom-right diagonal are taken
    if (board_tile_at(board, 0, 0)->symbol != ' ' &&
        board_tile_at(board, 1, 1)->symbol != ' ' &&
        board_tile_at(board, 2, 2)->symbol != ' ') {
        // if that diagonal is full with same symbol
        if (board_tile_at(board, 0, 0)->symbol == board_tile_at(board, 1, 1)->symbol &&
            board_tile_at(board, 2, 2)->symbol == board_tile_at(board, 1, 1)->symbol) {
            return board_tile_at(board, 0, 0)->symbol;
        }
    }

    // if the positions in the top-right to bottom-left diagonal are taken
    if (board_tile_at(board, 0, 2)->symbol != ' ' &&
        board_tile_at(board, 1, 1)->symbol != ' ' &&
        board_tile_at(board, 2, 0)->symbol != ' ') {
        // if that diagonal is full with same symbol
        if (board_tile_at(board, 0, 2)->symbol == board_tile_at(board, 1, 1)->symbol &&
            board_tile_at(board, 2, 0)->symbol == board_tile_at(board, 1, 1)->symbol) {
            return board_tile_at(board, 0, 2)->symbol;
        }
    }

    return ' ';
}
