/*
 * Starter scaffold, matching api.ts's own stub completion level: these
 * tests only characterize the hardcoded placeholder values the ports
 * currently return, the same way api.ts's handlers do before any real
 * domain logic exists. Replace a port's function pointer with a real
 * implementation (backed by a PlayerRepository / WorldRepository you
 * design) and update its test accordingly -- that's the kata.
 *
 * See README.md for the game rules (movement, items, gold, doors) that the
 * real implementation needs to satisfy.
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <string.h>

#include "kata.h"

static void register_player_returns_the_entrance_location(void** state) {
    (void)state;
    KatacombsPorts ports = katacombs_ports_create_stub();

    Player player = ports.register_player(&ports, "Ada");

    assert_string_equal(player.name, "Ada");
    assert_string_equal(player.location.description, "You are at the entrance");
    assert_int_equal(player.location.exit_count, 1);
    assert_int_equal(player.location.exits[0], DIRECTION_NORTH);
    assert_int_equal(player.bag.item_count, 0);
}

static void list_players_starts_empty(void** state) {
    (void)state;
    KatacombsPorts ports = katacombs_ports_create_stub();

    Player players[MAX_PLAYERS];
    size_t count = 123;
    ports.list_players(&ports, players, MAX_PLAYERS, &count);

    assert_int_equal(count, 0);
}

static void quit_game_says_good_bye_message(void** state) {
    (void)state;
    KatacombsPorts ports = katacombs_ports_create_stub();

    GameOverResult result = ports.quit_game(&ports, "some-player");

    assert_string_equal(result.message, "Game Over");
}

static void inspect_item_echoes_the_requested_sid(void** state) {
    (void)state;
    KatacombsPorts ports = katacombs_ports_create_stub();

    Item item = ports.inspect_item(&ports, "item-42");

    assert_string_equal(item.sid, "item-42");
    assert_string_equal(item.name, "Sample Item");
}

static void inspect_bag_starts_empty(void** state) {
    (void)state;
    KatacombsPorts ports = katacombs_ports_create_stub();

    BagResult result = ports.inspect_bag(&ports, "some-player");

    assert_string_equal(result.player_sid, "some-player");
    assert_int_equal(result.item_count, 0);
}

static void use_item_reports_success(void** state) {
    (void)state;
    KatacombsPorts ports = katacombs_ports_create_stub();

    UseItemResult result = ports.use_item(&ports, "some-player", "item-42", ACTION_OPEN);

    assert_string_equal(result.message, "Action completed successfully");
    assert_int_equal(result.action, ACTION_OPEN);
}

static void get_location_describes_a_room(void** state) {
    (void)state;
    KatacombsPorts ports = katacombs_ports_create_stub();

    LocationResult result = ports.get_location(&ports, "some-player");

    assert_string_equal(result.description, "You are in a room");
    assert_int_equal(result.exit_count, 2);
}

static void look_direction_describes_a_path(void** state) {
    (void)state;
    KatacombsPorts ports = katacombs_ports_create_stub();

    DirectionResult result = ports.look_direction(&ports, "some-player", DIRECTION_EAST);

    assert_int_equal(result.direction, DIRECTION_EAST);
    assert_string_equal(result.location.description, "You see a path leading somewhere");
}

static void move_direction_reports_success(void** state) {
    (void)state;
    KatacombsPorts ports = katacombs_ports_create_stub();

    MoveResult result = ports.move_direction(&ports, "some-player", DIRECTION_WEST);

    assert_int_equal(result.direction, DIRECTION_WEST);
    assert_string_equal(result.message, "Moved successfully");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(register_player_returns_the_entrance_location),
        cmocka_unit_test(list_players_starts_empty),
        cmocka_unit_test(quit_game_says_good_bye_message),
        cmocka_unit_test(inspect_item_echoes_the_requested_sid),
        cmocka_unit_test(inspect_bag_starts_empty),
        cmocka_unit_test(use_item_reports_success),
        cmocka_unit_test(get_location_describes_a_room),
        cmocka_unit_test(look_direction_describes_a_path),
        cmocka_unit_test(move_direction_reports_success),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
