#pragma once

/*
 * Legacy Code & Test Smells Kata - Yahtzee dice (C port).
 *
 * This header intentionally contains no explanatory comments about what is
 * wrong with it. Finding and naming each test smell in test_kata.c is part
 * of the exercise. See README.md.
 */

#include <stddef.h>

#define DICE_CUP_SIZE 5

/* ---- Die ----------------------------------------------------------------- */

typedef struct {
    int value;
} Die;

int die_equals(Die a, Die b);

/* ---- TelemetryPort (hexagonal-ish seam used by TurnLog) ------------------- */

typedef struct TelemetryPort {
    void (*record)(struct TelemetryPort* self, const char* entry);
} TelemetryPort;

/* ---- DiceCup --------------------------------------------------------------- */

typedef double (*RandomSource)(void);

typedef struct DiceCup {
    Die dice[DICE_CUP_SIZE];
    int dice_count;
    int selected_indexes[DICE_CUP_SIZE];
    int selected_count;
    RandomSource random_source;

    /* Swappable so a test can fake a "concrete class" instance, mirroring
     * the TS smell of casting a plain object to DiceCup (same pattern as
     * 21-smelly_mars_rover's CommandTranslator.translate_sequence_fn). Left
     * NULL to use the real implementation. out_count receives the length of
     * the returned array. */
    const Die* (*reroll_selected_fn)(struct DiceCup* self, int* out_count);
} DiceCup;

/* random_source may be NULL to use the default (rand()-based) source. */
DiceCup dice_cup_create(RandomSource random_source);

const Die* dice_cup_roll(DiceCup* cup, int* out_count);
void dice_cup_select_for_reroll(DiceCup* cup, const int* indexes, int count);
const Die* dice_cup_reroll_selected(DiceCup* cup, int* out_count);
const Die* dice_cup_current_dice(DiceCup* cup, int* out_count);

/* Exposed here (not kept file-static in kata.c) purely so a test can reach
 * into it directly -- the C analogue of the TypeScript "cast past
 * `private`" smell (see PORTING_NOTES_C.md in
 * ts-kata-answers/16_SmellyMarsRover for the same technique applied to
 * CommandTranslator's translate helpers). */
Die dice_cup_roll_die(DiceCup* cup);

/* ---- TurnLog ----------------------------------------------------------------- */

typedef struct {
    DiceCup* dice_cup;
    TelemetryPort* telemetry;
} TurnLog;

TurnLog turn_log_create(DiceCup* dice_cup, TelemetryPort* telemetry);

/* Caller does not own the returned array (it points into dice_cup's own
 * storage, or into whatever reroll_selected_fn returned). out_count
 * receives its length. */
const Die* turn_log_reroll_selected_dice(TurnLog* log, int* out_count);
