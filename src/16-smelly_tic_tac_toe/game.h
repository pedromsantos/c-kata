#pragma once

#include "board.h"

typedef struct {
    char last_symbol;
    Board board;
    char winner;
} Game;

Game* game_create(void);
void game_destroy(Game* game);
void game_play(Game* game, char symbol, int x, int y);
char game_determine_winner(Game* game);
