#pragma once

/*
 * Katacombs Kata (DDD / hexagonal, C port of api.ts).
 *
 * ts-kata's `src/24_Katacombs/api.ts` is itself only a 226-line stub: an
 * Express transport layer whose handlers all return hardcoded placeholder
 * values behind `// TODO: Implement ... logic` comments, wired to no real
 * domain model at all. This header/`.c` pair ports that same completion
 * level -- domain types plus a set of use-case ports whose default
 * implementation returns the same canned values api.ts did -- without a
 * transport layer, per the ts-kata README ("focus on use cases, transport
 * layer implementation not required").
 *
 * Ports are a single struct of function pointers (KatacombsPorts);
 * katacombs_ports_create_stub() wires the canned/default implementation.
 * Replacing individual function pointers with real use cases (backed by a
 * PlayerRepository / WorldRepository you design) is the kata.
 */

#include <stddef.h>

/* ---- Base types (api.ts: Direction, Action, Player, Location, Item, Bag) --- */

typedef enum { DIRECTION_NORTH, DIRECTION_SOUTH, DIRECTION_EAST, DIRECTION_WEST, DIRECTION_UP, DIRECTION_DOWN } Direction;

typedef enum { ACTION_OPEN, ACTION_CLOSE, ACTION_PICK, ACTION_DROP, ACTION_USE } Action;

#define SID_MAX 40
#define NAME_MAX 64
#define DESCRIPTION_MAX 256
#define MESSAGE_MAX 128
#define MAX_ACTIONS 5
#define MAX_ITEMS 16
#define MAX_EXITS 6
#define MAX_PLAYERS 8

typedef struct {
    char sid[SID_MAX];
    char name[NAME_MAX];
    char description[DESCRIPTION_MAX];
    Action actions[MAX_ACTIONS];
    int action_count;
} Item;

typedef struct {
    Item items[MAX_ITEMS];
    int item_count;
} Bag;

typedef struct {
    char description[DESCRIPTION_MAX];
    Direction exits[MAX_EXITS];
    int exit_count;
    Item items[MAX_ITEMS];
    int item_count;
} Location;

typedef struct {
    char sid[SID_MAX];
    char name[NAME_MAX];
    Location location;
    Bag bag;
} Player;

/* ---- Use-case results (api.ts: *Response interfaces) ----------------------- */

typedef struct {
    char message[MESSAGE_MAX];
} GameOverResult;

typedef struct {
    char player_sid[SID_MAX];
    Item items[MAX_ITEMS];
    int item_count;
} BagResult;

typedef struct {
    char player_sid[SID_MAX];
    char item_sid[SID_MAX];
    Action action;
    char message[MESSAGE_MAX];
} UseItemResult;

typedef struct {
    char player_sid[SID_MAX];
    char description[DESCRIPTION_MAX];
    Direction exits[MAX_EXITS];
    int exit_count;
    Item items[MAX_ITEMS];
    int item_count;
} LocationResult;

typedef struct {
    LocationResult location; /* api.ts: DirectionResponse extends LocationResponse */
    Direction direction;
} DirectionResult;

typedef struct {
    char player_sid[SID_MAX];
    Direction direction;
    char message[MESSAGE_MAX];
} MoveResult;

/* ---- Use-case ports (api.ts route handlers, minus the transport layer) ----- */

typedef struct KatacombsPorts {
    Player (*register_player)(struct KatacombsPorts* self, const char* name);
    void (*list_players)(struct KatacombsPorts* self, Player* out_players, size_t max_players, size_t* out_count);
    GameOverResult (*quit_game)(struct KatacombsPorts* self, const char* player_sid);
    Item (*inspect_item)(struct KatacombsPorts* self, const char* item_sid);
    BagResult (*inspect_bag)(struct KatacombsPorts* self, const char* player_sid);
    UseItemResult (*use_item)(struct KatacombsPorts* self, const char* player_sid, const char* item_sid,
                               Action action);
    LocationResult (*get_location)(struct KatacombsPorts* self, const char* player_sid);
    DirectionResult (*look_direction)(struct KatacombsPorts* self, const char* player_sid, Direction direction);
    MoveResult (*move_direction)(struct KatacombsPorts* self, const char* player_sid, Direction direction);
} KatacombsPorts;

/* Wires the same placeholder behavior as api.ts: every use case returns a
 * hardcoded value instead of consulting a real domain model. */
KatacombsPorts katacombs_ports_create_stub(void);
