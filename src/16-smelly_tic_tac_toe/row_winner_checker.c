#include "row_winner_checker.h"

char row_winner_checker_check(Board* board) {
    // if the positions in first row are taken
    if (board_tile_at(board, 0, 0)->symbol != ' ' &&
        board_tile_at(board, 0, 1)->symbol != ' ' &&
        board_tile_at(board, 0, 2)->symbol != ' ') {
        // if first row is full with same symbol
        if (board_tile_at(board, 0, 0)->symbol == board_tile_at(board, 0, 1)->symbol &&
            board_tile_at(board, 0, 2)->symbol == board_tile_at(board, 0, 1)->symbol) {
            return board_tile_at(board, 0, 0)->symbol;
        }
    }

    // if the positions in middle row are taken
    if (board_tile_at(board, 1, 0)->symbol != ' ' &&
        board_tile_at(board, 1, 1)->symbol != ' ' &&
        board_tile_at(board, 1, 2)->symbol != ' ') {
        // if middle row is full with same symbol
        if (board_tile_at(board, 1, 0)->symbol == board_tile_at(board, 1, 1)->symbol &&
            board_tile_at(board, 1, 2)->symbol == board_tile_at(board, 1, 1)->symbol) {
            return board_tile_at(board, 1, 0)->symbol;
        }
    }

    // if the positions in last row are taken
    if (board_tile_at(board, 2, 0)->symbol != ' ' &&
        board_tile_at(board, 2, 1)->symbol != ' ' &&
        board_tile_at(board, 2, 2)->symbol != ' ') {
        // if last row is full with same symbol
        if (board_tile_at(board, 2, 0)->symbol == board_tile_at(board, 2, 1)->symbol &&
            board_tile_at(board, 2, 2)->symbol == board_tile_at(board, 2, 1)->symbol) {
            return board_tile_at(board, 2, 0)->symbol;
        }
    }

    return ' ';
}
