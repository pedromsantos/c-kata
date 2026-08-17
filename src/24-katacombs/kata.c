#include "kata.h"

#include <stdio.h>
#include <string.h>

/* Each function below is a direct, non-transport port of the matching
 * api.ts Express handler body -- same hardcoded placeholder values, same
 * "// TODO: Implement ... logic" gaps. */

static Player stub_register_player(KatacombsPorts* self, const char* name) {
    (void)self;
    /* TODO: Implement game start logic */
    Player player;
    memset(&player, 0, sizeof(player));
    snprintf(player.sid, sizeof(player.sid), "000000-000000000000-00000000");
    snprintf(player.name, sizeof(player.name), "%s", name);
    snprintf(player.location.description, sizeof(player.location.description), "You are at the entrance");
    player.location.exits[0] = DIRECTION_NORTH;
    player.location.exit_count = 1;
    player.location.item_count = 0;
    player.bag.item_count = 0;
    return player;
}

static void stub_list_players(KatacombsPorts* self, Player* out_players, size_t max_players, size_t* out_count) {
    (void)self;
    (void)out_players;
    (void)max_players;
    /* TODO: Implement list players logic */
    *out_count = 0;
}

static GameOverResult stub_quit_game(KatacombsPorts* self, const char* player_sid) {
    (void)self;
    (void)player_sid;
    /* TODO: Implement quit game logic */
    GameOverResult result;
    snprintf(result.message, sizeof(result.message), "Game Over");
    return result;
}

static Item stub_inspect_item(KatacombsPorts* self, const char* item_sid) {
    (void)self;
    /* TODO: Implement item inspection logic using item_sid */
    Item item;
    memset(&item, 0, sizeof(item));
    snprintf(item.sid, sizeof(item.sid), "%s", item_sid);
    snprintf(item.name, sizeof(item.name), "Sample Item");
    snprintf(item.description, sizeof(item.description), "Sample Description");
    item.actions[0] = ACTION_OPEN;
    item.actions[1] = ACTION_CLOSE;
    item.action_count = 2;
    return item;
}

static BagResult stub_inspect_bag(KatacombsPorts* self, const char* player_sid) {
    (void)self;
    /* TODO: Implement bag content logic using player_sid */
    BagResult result;
    snprintf(result.player_sid, sizeof(result.player_sid), "%s", player_sid);
    result.item_count = 0;
    return result;
}

static UseItemResult stub_use_item(KatacombsPorts* self, const char* player_sid, const char* item_sid,
                                    Action action) {
    (void)self;
    /* TODO: Implement use item logic using player_sid, item_sid, and action */
    UseItemResult result;
    snprintf(result.player_sid, sizeof(result.player_sid), "%s", player_sid);
    snprintf(result.item_sid, sizeof(result.item_sid), "%s", item_sid);
    result.action = action;
    snprintf(result.message, sizeof(result.message), "Action completed successfully");
    return result;
}

static LocationResult stub_get_location(KatacombsPorts* self, const char* player_sid) {
    (void)self;
    /* TODO: Implement look around logic using player_sid */
    LocationResult result;
    snprintf(result.player_sid, sizeof(result.player_sid), "%s", player_sid);
    snprintf(result.description, sizeof(result.description), "You are in a room");
    result.exits[0] = DIRECTION_NORTH;
    result.exits[1] = DIRECTION_SOUTH;
    result.exit_count = 2;
    result.item_count = 0;
    return result;
}

static DirectionResult stub_look_direction(KatacombsPorts* self, const char* player_sid, Direction direction) {
    (void)self;
    /* TODO: Implement look direction logic using player_sid and direction */
    DirectionResult result;
    snprintf(result.location.player_sid, sizeof(result.location.player_sid), "%s", player_sid);
    result.direction = direction;
    snprintf(result.location.description, sizeof(result.location.description),
             "You see a path leading somewhere");
    result.location.exit_count = 0;
    result.location.item_count = 0;
    return result;
}

static MoveResult stub_move_direction(KatacombsPorts* self, const char* player_sid, Direction direction) {
    (void)self;
    /* TODO: Implement move logic using player_sid and direction */
    MoveResult result;
    snprintf(result.player_sid, sizeof(result.player_sid), "%s", player_sid);
    result.direction = direction;
    snprintf(result.message, sizeof(result.message), "Moved successfully");
    return result;
}

KatacombsPorts katacombs_ports_create_stub(void) {
    KatacombsPorts ports;
    ports.register_player = stub_register_player;
    ports.list_players = stub_list_players;
    ports.quit_game = stub_quit_game;
    ports.inspect_item = stub_inspect_item;
    ports.inspect_bag = stub_inspect_bag;
    ports.use_item = stub_use_item;
    ports.get_location = stub_get_location;
    ports.look_direction = stub_look_direction;
    ports.move_direction = stub_move_direction;
    return ports;
}
