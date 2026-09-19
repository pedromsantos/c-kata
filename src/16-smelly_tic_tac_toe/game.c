#include "game.h"
#include "column_winner_checker.h"
#include "diagonal_winner_checker.h"
#include "row_winner_checker.h"
#include <stdio.h>
#include <stdlib.h>

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
    char row_winner = row_winner_checker_check(&game->board);
    if (row_winner != ' ') {
        return row_winner;
    }

    char column_winner = column_winner_checker_check(&game->board);
    if (column_winner != ' ') {
        return column_winner;
    }

    return diagonal_winner_checker_check(&game->board);
}
