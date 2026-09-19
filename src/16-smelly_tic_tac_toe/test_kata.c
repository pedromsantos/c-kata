/*
 * Verification-fixture tests, ported from the TypeScript reference
 * (Board.ts, RowWinnerChecker.ts, ColumnWinnerChecker.ts,
 * DiagonalWinnerChecker.ts, Game.ts, test.ts in ts-kata's
 * 15_SmellyTicTacToe). See README.md.
 *
 * game_play() has no exceptions in C: an invalid move is rejected by
 * logging to stderr and returning without mutating state (see game.c).
 * The "does not allow ..." tests below assert that rejection the C way,
 * by checking that the game's last_symbol and the targeted tile are left
 * exactly as they were before the rejected play.
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include "game.h"

/* ==== Game::Play validation ==================================================== */

static void does_not_allow_player_o_to_play_first(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'O', 0, 0);

    assert_int_equal(game->last_symbol, ' ');
    assert_int_equal(board_tile_at(&game->board, 0, 0)->symbol, ' ');

    game_destroy(game);
}

static void does_not_allow_player_to_play_twice_in_a_row(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 0, 0);
    game_play(game, 'X', 1, 0);

    assert_int_equal(game->last_symbol, 'X');
    assert_int_equal(board_tile_at(&game->board, 1, 0)->symbol, ' ');

    game_destroy(game);
}

static void does_not_allow_play_in_last_played_position(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 0, 0);

    assert_int_equal(game->last_symbol, 'X');
    assert_int_equal(board_tile_at(&game->board, 0, 0)->symbol, 'X');

    game_destroy(game);
}

static void does_not_allow_play_in_any_played_position(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 1, 0);
    game_play(game, 'X', 0, 0);

    assert_int_equal(game->last_symbol, 'O');
    assert_int_equal(board_tile_at(&game->board, 0, 0)->symbol, 'X');

    game_destroy(game);
}

/* ==== Game::Winner -- rows ====================================================== */

static void x_wins_top_row(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 1, 0);
    game_play(game, 'X', 0, 1);
    game_play(game, 'O', 1, 1);
    game_play(game, 'X', 0, 2);

    assert_int_equal(game_determine_winner(game), 'X');

    game_destroy(game);
}

static void o_wins_top_row(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 1, 0);
    game_play(game, 'O', 0, 0);
    game_play(game, 'X', 1, 1);
    game_play(game, 'O', 0, 1);
    game_play(game, 'X', 2, 2);
    game_play(game, 'O', 0, 2);

    assert_int_equal(game_determine_winner(game), 'O');

    game_destroy(game);
}

static void x_wins_middle_row(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 1, 0);
    game_play(game, 'O', 0, 0);
    game_play(game, 'X', 1, 1);
    game_play(game, 'O', 0, 1);
    game_play(game, 'X', 1, 2);

    assert_int_equal(game_determine_winner(game), 'X');

    game_destroy(game);
}

static void o_wins_middle_row(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 1, 0);
    game_play(game, 'X', 2, 1);
    game_play(game, 'O', 1, 1);
    game_play(game, 'X', 2, 2);
    game_play(game, 'O', 1, 2);

    assert_int_equal(game_determine_winner(game), 'O');

    game_destroy(game);
}

static void x_wins_bottom_row(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 2, 0);
    game_play(game, 'O', 0, 0);
    game_play(game, 'X', 2, 1);
    game_play(game, 'O', 0, 1);
    game_play(game, 'X', 2, 2);

    assert_int_equal(game_determine_winner(game), 'X');

    game_destroy(game);
}

static void o_wins_bottom_row(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 2, 0);
    game_play(game, 'X', 1, 1);
    game_play(game, 'O', 2, 1);
    game_play(game, 'X', 0, 1);
    game_play(game, 'O', 2, 2);

    assert_int_equal(game_determine_winner(game), 'O');

    game_destroy(game);
}

/* ==== Game::Winner -- columns ==================================================== */

static void x_wins_a_column(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 0, 1);
    game_play(game, 'X', 1, 0);
    game_play(game, 'O', 1, 1);
    game_play(game, 'X', 2, 0);

    assert_int_equal(game_determine_winner(game), 'X');

    game_destroy(game);
}

static void o_wins_a_column(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 0, 1);
    game_play(game, 'O', 0, 0);
    game_play(game, 'X', 1, 1);
    game_play(game, 'O', 1, 0);
    game_play(game, 'X', 0, 2);
    game_play(game, 'O', 2, 0);

    assert_int_equal(game_determine_winner(game), 'O');

    game_destroy(game);
}

/* ==== Game::Winner -- diagonals ================================================== */

static void x_wins_diagonal(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 0, 1);
    game_play(game, 'X', 1, 1);
    game_play(game, 'O', 1, 0);
    game_play(game, 'X', 2, 2);

    assert_int_equal(game_determine_winner(game), 'X');

    game_destroy(game);
}

static void o_wins_anti_diagonal(void** state) {
    (void)state;
    Game* game = game_create();

    game_play(game, 'X', 0, 1);
    game_play(game, 'O', 0, 2);
    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 1, 1);
    game_play(game, 'X', 1, 0);
    game_play(game, 'O', 2, 0);

    assert_int_equal(game_determine_winner(game), 'O');

    game_destroy(game);
}

int main(void) {
    const struct CMUnitTest validation_tests[] = {
        cmocka_unit_test(does_not_allow_player_o_to_play_first),
        cmocka_unit_test(does_not_allow_player_to_play_twice_in_a_row),
        cmocka_unit_test(does_not_allow_play_in_last_played_position),
        cmocka_unit_test(does_not_allow_play_in_any_played_position),
    };

    const struct CMUnitTest row_tests[] = {
        cmocka_unit_test(x_wins_top_row),
        cmocka_unit_test(o_wins_top_row),
        cmocka_unit_test(x_wins_middle_row),
        cmocka_unit_test(o_wins_middle_row),
        cmocka_unit_test(x_wins_bottom_row),
        cmocka_unit_test(o_wins_bottom_row),
    };

    const struct CMUnitTest column_tests[] = {
        cmocka_unit_test(x_wins_a_column),
        cmocka_unit_test(o_wins_a_column),
    };

    const struct CMUnitTest diagonal_tests[] = {
        cmocka_unit_test(x_wins_diagonal),
        cmocka_unit_test(o_wins_anti_diagonal),
    };

    int failed = 0;
    failed += cmocka_run_group_tests(validation_tests, NULL, NULL);
    failed += cmocka_run_group_tests(row_tests, NULL, NULL);
    failed += cmocka_run_group_tests(column_tests, NULL, NULL);
    failed += cmocka_run_group_tests(diagonal_tests, NULL, NULL);
    return failed;
}
