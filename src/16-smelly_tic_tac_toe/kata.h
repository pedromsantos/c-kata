#pragma once

typedef struct {
    int x;
    int y;
    char symbol;
} Tile;

typedef struct {
    Tile plays[9];
} Board;

typedef struct {
    char last_symbol;
    Board board;
    char winner;
} Game;

Tile tile_create(int x, int y);

Board* board_create(void);
void board_destroy(Board* board);
Tile* board_tile_at(Board* board, int x, int y);
void board_add_tile_at(Board* board, char symbol, int x, int y);

Game* game_create(void);
void game_destroy(Game* game);
void game_play(Game* game, char symbol, int x, int y);
char game_determine_winner(Game* game);
