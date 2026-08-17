#include "kata.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- Die ------------------------------------------------------------------ */

int die_equals(Die a, Die b) {
    return a.value == b.value;
}

/* ---- DiceCup ---------------------------------------------------------------- */

static double default_random_source(void) {
    return (double)rand() / ((double)RAND_MAX + 1.0);
}

static Die dice_cup_roll_die_internal(DiceCup* cup) {
    Die die;
    die.value = (int)(cup->random_source() * 6) + 1;
    return die;
}

Die dice_cup_roll_die(DiceCup* cup) {
    return dice_cup_roll_die_internal(cup);
}

DiceCup dice_cup_create(RandomSource random_source) {
    DiceCup cup;
    memset(&cup, 0, sizeof(cup));
    cup.random_source = random_source ? random_source : default_random_source;
    cup.dice_count = 0;
    cup.selected_count = 0;
    cup.reroll_selected_fn = NULL;
    return cup;
}

const Die* dice_cup_roll(DiceCup* cup, int* out_count) {
    for (int i = 0; i < DICE_CUP_SIZE; i++) {
        cup->dice[i] = dice_cup_roll_die_internal(cup);
    }
    cup->dice_count = DICE_CUP_SIZE;
    cup->selected_count = 0;

    if (out_count) *out_count = cup->dice_count;
    return cup->dice;
}

void dice_cup_select_for_reroll(DiceCup* cup, const int* indexes, int count) {
    int n = count < DICE_CUP_SIZE ? count : DICE_CUP_SIZE;
    for (int i = 0; i < n; i++) {
        cup->selected_indexes[i] = indexes[i];
    }
    cup->selected_count = n;
}

static const Die* dice_cup_reroll_selected_default(DiceCup* cup, int* out_count) {
    for (int i = 0; i < cup->selected_count; i++) {
        int index = cup->selected_indexes[i];
        cup->dice[index] = dice_cup_roll_die_internal(cup);
    }
    cup->selected_count = 0;

    if (out_count) *out_count = cup->dice_count;
    return cup->dice;
}

const Die* dice_cup_reroll_selected(DiceCup* cup, int* out_count) {
    if (cup->reroll_selected_fn) {
        return cup->reroll_selected_fn(cup, out_count);
    }
    return dice_cup_reroll_selected_default(cup, out_count);
}

const Die* dice_cup_current_dice(DiceCup* cup, int* out_count) {
    if (out_count) *out_count = cup->dice_count;
    return cup->dice;
}

/* ---- TurnLog ------------------------------------------------------------------ */

TurnLog turn_log_create(DiceCup* dice_cup, TelemetryPort* telemetry) {
    TurnLog log;
    log.dice_cup = dice_cup;
    log.telemetry = telemetry;
    return log;
}

const Die* turn_log_reroll_selected_dice(TurnLog* log, int* out_count) {
    int count = 0;
    const Die* dice = dice_cup_reroll_selected(log->dice_cup, &count);

    char entry[256];
    int offset = snprintf(entry, sizeof(entry), "rerolled:");
    for (int i = 0; i < count; i++) {
        if (i > 0) {
            offset += snprintf(entry + offset, sizeof(entry) - (size_t)offset, ",");
        }
        offset += snprintf(entry + offset, sizeof(entry) - (size_t)offset, "%d", dice[i].value);
    }

    log->telemetry->record(log->telemetry, entry);

    if (out_count) *out_count = count;
    return dice;
}
