#include "board.h"
#include <stdlib.h>

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
