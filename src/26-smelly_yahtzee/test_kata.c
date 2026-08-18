/*
 * Deliberately smelly characterization tests, ported from the TypeScript
 * fixture. See README.md and ts-kata's 17_SmellyYahtzee for background --
 * not to be "fixed" here, this file matches ts-kata's own "currently
 * passes, but is deliberately smelly" starting point.
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "kata.h"

/* ---- DiceCup: shared mutable state across tests (smell) ------------------ */

static DiceCup shared_cup;
static int roll_count = 0;
static time_t test_run_timestamp;

static double zero_random(void) {
    return 0;
}

static double half_random(void) {
    return 0.5;
}

static int setup_shared_cup(void** state) {
    (void)state;
    shared_cup = dice_cup_create(zero_random);
    return 0;
}

static void test1(void** state) {
    (void)state;
    roll_count++;
    const Die* dice = dice_cup_roll(&shared_cup, NULL);
    assert_non_null(dice); /* Testing Theater: never asserted on beyond this. */
}

static void should_work(void** state) {
    (void)state;
    /* Test Interdependence: relies on test1() having already run. */
    assert_true(roll_count > 0);

    int count = 0;
    dice_cup_current_dice(&shared_cup, &count);
    assert_int_equal(count, DICE_CUP_SIZE);
}

static void rolls_selects_rerolls_and_clears_selection(void** state) {
    (void)state;
    /* Eager Test: exercises roll + select + reroll + clear in one test. */
    DiceCup cup = dice_cup_create(half_random);
    int rolled_count = 0;
    const Die* rolled = dice_cup_roll(&cup, &rolled_count);

    int indexes[] = {0, 2};
    dice_cup_select_for_reroll(&cup, indexes, 2);
    int rerolled_count = 0;
    const Die* rerolled = dice_cup_reroll_selected(&cup, &rerolled_count);

    assert_int_equal(rolled_count, DICE_CUP_SIZE);
    assert_int_equal(rolled[0].value, 4);
    assert_int_equal(rolled[1].value, 4);
    assert_int_equal(rerolled[2].value, 4);

    int current_count = 0;
    const Die* current = dice_cup_current_dice(&cup, &current_count);
    assert_int_equal(current_count, DICE_CUP_SIZE);
    assert_ptr_equal(rerolled, current);
}

static void does_things(void** state) {
    (void)state;
    /* Obscure Test: name says nothing about the behavior under test. */
    DiceCup cup = dice_cup_create(zero_random);
    int count = 0;
    const Die* dice = dice_cup_roll(&cup, &count);

    assert_int_equal(count, DICE_CUP_SIZE);
    assert_int_equal(dice[0].value, 1);
    assert_int_equal(dice[1].value, 1);
    assert_int_equal(dice[2].value, 1);
    assert_int_equal(dice[3].value, 1);
}

/* A stateful RandomSource: C function pointers carry no captured context
 * (unlike the TS closure `() => values.shift()!`), so the queue has to live
 * in file-scoped static storage instead. */
static double queued_values[DICE_CUP_SIZE];
static int queued_index = 0;

static double queued_random(void) {
    return queued_values[queued_index++];
}

static void computes_expected_dice_with_same_branching_as_cup(void** state) {
    (void)state;
    /* Logic in Test / Fragile Test: re-implements production branching. */
    double values[DICE_CUP_SIZE] = {0.01, 0.2, 0.4, 0.7, 0.99};
    memcpy(queued_values, values, sizeof(values));
    queued_index = 0;

    DiceCup cup = dice_cup_create(queued_random);

    int expected[DICE_CUP_SIZE];
    for (int i = 0; i < DICE_CUP_SIZE; i++) {
        double value = values[i];
        if (value < 1.0 / 6) expected[i] = 1;
        else if (value < 2.0 / 6) expected[i] = 2;
        else if (value < 3.0 / 6) expected[i] = 3;
        else if (value < 5.0 / 6) expected[i] = 5;
        else expected[i] = 6;
    }

    int count = 0;
    const Die* dice = dice_cup_roll(&cup, &count);
    for (int i = 0; i < count; i++) {
        assert_int_equal(dice[i].value, expected[i]);
    }
}

static void reaches_into_the_private_die_roller(void** state) {
    (void)state;
    /* Testing Private Methods: kata.h exposes an implementation-only
     * helper purely so this test can call it directly. */
    DiceCup cup = dice_cup_create(zero_random);
    Die die = dice_cup_roll_die(&cup);
    assert_int_equal(die.value, 1);
}

static void slowly_waits_before_rolling(void** state) {
    (void)state;
    /* Slow Unit Test: a real sleep for no behavioral reason. */
    usleep(20 * 1000);
    DiceCup cup = dice_cup_create(zero_random);
    const Die* dice = dice_cup_roll(&cup, NULL);
    assert_int_equal(dice[0].value, 1);
}

static void rerolls_first_die_duplicate_case_one(void** state) {
    (void)state;
    DiceCup cup = dice_cup_create(zero_random);
    dice_cup_roll(&cup, NULL);
    int indexes[] = {0};
    dice_cup_select_for_reroll(&cup, indexes, 1);
    const Die* dice = dice_cup_reroll_selected(&cup, NULL);
    assert_int_equal(dice[0].value, 1);
}

static void rerolls_first_die_duplicate_case_two(void** state) {
    (void)state;
    DiceCup cup = dice_cup_create(zero_random);
    dice_cup_roll(&cup, NULL);
    int indexes[] = {0};
    dice_cup_select_for_reroll(&cup, indexes, 1);
    const Die* dice = dice_cup_reroll_selected(&cup, NULL);
    assert_int_equal(dice[0].value, 1);
}

static void rerolls_first_die_duplicate_case_three(void** state) {
    (void)state;
    DiceCup cup = dice_cup_create(zero_random);
    dice_cup_roll(&cup, NULL);
    int indexes[] = {0};
    dice_cup_select_for_reroll(&cup, indexes, 1);
    const Die* dice = dice_cup_reroll_selected(&cup, NULL);
    assert_int_equal(dice[0].value, 1);
}

/* ---- TurnLog: mock overuse ------------------------------------------------ */

static Die mock_reroll_dice[2];
static int mock_reroll_call_count = 0;

static const Die* mock_reroll_selected(DiceCup* self, int* out_count) {
    (void)self;
    /* Mocking a concrete class: fakes DiceCup's behavior instead of
     * exercising the real rolling logic; also returns a shorter array
     * (length 2) than a real DiceCup ever would (length 5). */
    mock_reroll_call_count++;
    mock_reroll_dice[0].value = 1;
    mock_reroll_dice[1].value = 2;
    if (out_count) *out_count = 2;
    return mock_reroll_dice;
}

static int telemetry_record_calls = 0;
static char last_recorded_entry[128];

static void mock_telemetry_record(TelemetryPort* self, const char* entry) {
    (void)self;
    telemetry_record_calls++;
    snprintf(last_recorded_entry, sizeof(last_recorded_entry), "%s", entry);
}

static void logs_rerolled_dice(void** state) {
    (void)state;
    DiceCup mock_cup = dice_cup_create(zero_random);
    mock_cup.reroll_selected_fn = mock_reroll_selected;
    mock_reroll_call_count = 0;

    telemetry_record_calls = 0;
    TelemetryPort mock_telemetry = {.record = mock_telemetry_record};

    /* Mocking Value Objects: a Die is built and asserted on trivially even
     * though TurnLog never touches it (Mystery Guest / Testing Theater). */
    Die mock_die = {6};

    TurnLog log = turn_log_create(&mock_cup, &mock_telemetry);
    int count = 0;
    const Die* dice = turn_log_reroll_selected_dice(&log, &count);

    assert_int_equal(count, 2);
    assert_int_equal(dice[0].value, 1);
    assert_int_equal(dice[1].value, 2);
    assert_string_equal(last_recorded_entry, "rerolled:1,2");
    assert_int_equal(telemetry_record_calls, 1);
    assert_int_equal(mock_reroll_call_count, 1);
    assert_int_equal(mock_die.value, 6);
}

static void records_a_timestamp_that_is_always_in_the_past(void** state) {
    (void)state;
    /* Obscure Test: name promises telemetry behavior but this doesn't
     * touch TurnLog or TelemetryPort at all. */
    assert_true(test_run_timestamp <= time(NULL));
}

int main(void) {
    test_run_timestamp = time(NULL);

    const struct CMUnitTest dice_cup_tests[] = {
        cmocka_unit_test_setup(test1, setup_shared_cup),
        cmocka_unit_test(should_work),
        cmocka_unit_test(rolls_selects_rerolls_and_clears_selection),
        cmocka_unit_test(does_things),
        cmocka_unit_test(computes_expected_dice_with_same_branching_as_cup),
        cmocka_unit_test(reaches_into_the_private_die_roller),
        cmocka_unit_test(slowly_waits_before_rolling),
        cmocka_unit_test(rerolls_first_die_duplicate_case_one),
        cmocka_unit_test(rerolls_first_die_duplicate_case_two),
        cmocka_unit_test(rerolls_first_die_duplicate_case_three),
    };

    const struct CMUnitTest turn_log_tests[] = {
        cmocka_unit_test(logs_rerolled_dice),
        cmocka_unit_test(records_a_timestamp_that_is_always_in_the_past),
    };

    int failed = 0;
    failed += cmocka_run_group_tests(dice_cup_tests, NULL, NULL);
    failed += cmocka_run_group_tests(turn_log_tests, NULL, NULL);
    return failed;
}
