#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_not_allow_player_o_to_play_first(void **state) {
    (void) state; /* unused */
    Game* game = game_create();
    game_play(game, 'O', 0, 0);
    /* In C we can't easily test exceptions, so we just verify the state didn't change */
    assert_int_equal(game->last_symbol, ' ');
    game_destroy(game);
}

static void test_not_allow_player_x_to_play_twice_in_a_row(void **state) {
    (void) state; /* unused */
    Game* game = game_create();
    game_play(game, 'X', 0, 0);
    char last = game->last_symbol;
    game_play(game, 'X', 1, 0);
    assert_int_equal(game->last_symbol, last);
    game_destroy(game);
}

static void test_not_allow_player_to_play_in_last_played_position(void **state) {
    (void) state; /* unused */
    Game* game = game_create();
    game_play(game, 'X', 0, 0);
    char last = game->last_symbol;
    game_play(game, 'O', 0, 0);
    assert_int_equal(game->last_symbol, last);
    game_destroy(game);
}

static void test_not_allow_player_to_play_in_any_played_position(void **state) {
    (void) state; /* unused */
    Game* game = game_create();
    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 1, 0);
    char last = game->last_symbol;
    game_play(game, 'X', 0, 0);
    assert_int_equal(game->last_symbol, last);
    game_destroy(game);
}

static void test_declare_player_x_as_a_winner_if_three_in_top_row(void **state) {
    (void) state; /* unused */
    Game* game = game_create();
    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 1, 0);
    game_play(game, 'X', 0, 1);
    game_play(game, 'O', 1, 1);
    game_play(game, 'X', 0, 2);
    char winner = game_determine_winner(game);
    assert_int_equal(winner, 'X');
    game_destroy(game);
}

static void test_declare_player_o_as_a_winner_if_three_in_top_row(void **state) {
    (void) state; /* unused */
    Game* game = game_create();
    game_play(game, 'X', 2, 2);
    game_play(game, 'O', 0, 0);
    game_play(game, 'X', 1, 0);
    game_play(game, 'O', 0, 1);
    game_play(game, 'X', 1, 1);
    game_play(game, 'O', 0, 2);
    char winner = game_determine_winner(game);
    assert_int_equal(winner, 'O');
    game_destroy(game);
}

static void test_declare_player_x_as_a_winner_if_three_in_middle_row(void **state) {
    (void) state; /* unused */
    Game* game = game_create();
    game_play(game, 'X', 1, 0);
    game_play(game, 'O', 0, 0);
    game_play(game, 'X', 1, 1);
    game_play(game, 'O', 0, 1);
    game_play(game, 'X', 1, 2);
    char winner = game_determine_winner(game);
    assert_int_equal(winner, 'X');
    game_destroy(game);
}

static void test_declare_player_o_as_a_winner_if_three_in_middle_row(void **state) {
    (void) state; /* unused */
    Game* game = game_create();
    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 1, 0);
    game_play(game, 'X', 2, 0);
    game_play(game, 'O', 1, 1);
    game_play(game, 'X', 2, 1);
    game_play(game, 'O', 1, 2);
    char winner = game_determine_winner(game);
    assert_int_equal(winner, 'O');
    game_destroy(game);
}

static void test_declare_player_x_as_a_winner_if_three_in_bottom_row(void **state) {
    (void) state; /* unused */
    Game* game = game_create();
    game_play(game, 'X', 2, 0);
    game_play(game, 'O', 0, 0);
    game_play(game, 'X', 2, 1);
    game_play(game, 'O', 0, 1);
    game_play(game, 'X', 2, 2);
    char winner = game_determine_winner(game);
    assert_int_equal(winner, 'X');
    game_destroy(game);
}

static void test_declare_player_o_as_a_winner_if_three_in_bottom_row(void **state) {
    (void) state; /* unused */
    Game* game = game_create();
    game_play(game, 'X', 0, 0);
    game_play(game, 'O', 2, 0);
    game_play(game, 'X', 1, 0);
    game_play(game, 'O', 2, 1);
    game_play(game, 'X', 1, 1);
    game_play(game, 'O', 2, 2);
    char winner = game_determine_winner(game);
    assert_int_equal(winner, 'O');
    game_destroy(game);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_not_allow_player_o_to_play_first),
        cmocka_unit_test(test_not_allow_player_x_to_play_twice_in_a_row),
        cmocka_unit_test(test_not_allow_player_to_play_in_last_played_position),
        cmocka_unit_test(test_not_allow_player_to_play_in_any_played_position),
        cmocka_unit_test(test_declare_player_x_as_a_winner_if_three_in_top_row),
        cmocka_unit_test(test_declare_player_o_as_a_winner_if_three_in_top_row),
        cmocka_unit_test(test_declare_player_x_as_a_winner_if_three_in_middle_row),
        cmocka_unit_test(test_declare_player_o_as_a_winner_if_three_in_middle_row),
        cmocka_unit_test(test_declare_player_x_as_a_winner_if_three_in_bottom_row),
        cmocka_unit_test(test_declare_player_o_as_a_winner_if_three_in_bottom_row),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
