#include "kata.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Tile tile_create(int x, int y) {
    Tile tile;
    tile.x = x;
    tile.y = y;
    tile.symbol = ' ';
    return tile;
}

Board* board_create(void) {
    Board* board = malloc(sizeof(Board));
    if (board) {
        int index = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board->plays[index++] = tile_create(i, j);
            }
        }
    }
    return board;
}

void board_destroy(Board* board) {
    if (board) {
        free(board);
    }
}

Tile* board_tile_at(Board* board, int x, int y) {
    for (int i = 0; i < 9; i++) {
        if (board->plays[i].x == x && board->plays[i].y == y) {
            return &board->plays[i];
        }
    }
    return NULL;
}

void board_add_tile_at(Board* board, char symbol, int x, int y) {
    Tile* tile = board_tile_at(board, x, y);
    if (tile) {
        tile->symbol = symbol;
    }
}

Game* game_create(void) {
    Game* game = malloc(sizeof(Game));
    if (game) {
        game->last_symbol = ' ';
        int index = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                game->board.plays[index++] = tile_create(i, j);
            }
        }
        game->winner = ' ';
    }
    return game;
}

void game_destroy(Game* game) {
    if (game) {
        free(game);
    }
}

void game_play(Game* game, char symbol, int x, int y) {
    // if first move
    if (game->last_symbol == ' ') {
        // if player is O
        if (symbol == 'O') {
            fprintf(stderr, "Invalid first player\n");
            return;
        }
    }
    // if not first move but player repeated
    else if (symbol == game->last_symbol) {
        fprintf(stderr, "Invalid next player\n");
        return;
    }
    // if not first move but play on an already played tile
    else if (board_tile_at(&game->board, x, y)->symbol != ' ') {
        fprintf(stderr, "Invalid position\n");
        return;
    }

    // update game state
    game->last_symbol = symbol;
    board_add_tile_at(&game->board, symbol, x, y);
}

char game_determine_winner(Game* game) {
    // if the positions in first row are taken
    if (board_tile_at(&game->board, 0, 0)->symbol != ' ' &&
        board_tile_at(&game->board, 0, 1)->symbol != ' ' &&
        board_tile_at(&game->board, 0, 2)->symbol != ' ') {
        // if first row is full with same symbol
        if (board_tile_at(&game->board, 0, 0)->symbol == board_tile_at(&game->board, 0, 1)->symbol &&
            board_tile_at(&game->board, 0, 2)->symbol == board_tile_at(&game->board, 0, 1)->symbol) {
            return board_tile_at(&game->board, 0, 0)->symbol;
        }
    }

    // if the positions in middle row are taken
    if (board_tile_at(&game->board, 1, 0)->symbol != ' ' &&
        board_tile_at(&game->board, 1, 1)->symbol != ' ' &&
        board_tile_at(&game->board, 1, 2)->symbol != ' ') {
        // if middle row is full with same symbol
        if (board_tile_at(&game->board, 1, 0)->symbol == board_tile_at(&game->board, 1, 1)->symbol &&
            board_tile_at(&game->board, 1, 2)->symbol == board_tile_at(&game->board, 1, 1)->symbol) {
            return board_tile_at(&game->board, 1, 0)->symbol;
        }
    }

    // if the positions in last row are taken
    if (board_tile_at(&game->board, 2, 0)->symbol != ' ' &&
        board_tile_at(&game->board, 2, 1)->symbol != ' ' &&
        board_tile_at(&game->board, 2, 2)->symbol != ' ') {
        // if last row is full with same symbol
        if (board_tile_at(&game->board, 2, 0)->symbol == board_tile_at(&game->board, 2, 1)->symbol &&
            board_tile_at(&game->board, 2, 2)->symbol == board_tile_at(&game->board, 2, 1)->symbol) {
            return board_tile_at(&game->board, 2, 0)->symbol;
        }
    }

    return ' ';
}
