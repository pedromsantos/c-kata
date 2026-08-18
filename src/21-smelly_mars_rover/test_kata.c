/*
 * Deliberately smelly characterization tests, ported from the TypeScript
 * fixture. See README.md and ts-kata-answers/16_SmellyMarsRover for
 * background (not to be copied wholesale -- this file matches the same
 * "currently passes, but is deliberately smelly" starting point).
 *
 * Note what is NOT tested here at all: Rover, ObstacleSensor,
 * RadioTransmitter, MissionClock. That is intentional -- see README.md.
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

/* ---- CommandTranslator: shared mutable state across tests (smell) ------- */

static CommandTranslator shared_translator;
static int call_count = 0;
static time_t test_run_timestamp;

static int setup_shared_state(void** state) {
    (void)state;
    shared_translator = command_translator_create();
    return 0;
}

static void test1(void** state) {
    (void)state;
    call_count++;
    char result = command_translator_translate(&shared_translator, 'I', "ES");
    (void)result; /* Testing Theater: never asserted on */
    assert_true(1);
}

static void should_work(void** state) {
    (void)state;
    /* Test Interdependence: relies on test1() having already run. */
    assert_true(call_count > 0);
    assert_string_equal(command_translator_get_last_language_used(), "ES");
}

static void translates_and_logs_and_reports_and_handles_unknown_and_sequences(void** state) {
    (void)state;
    CommandTranslator t = command_translator_create();

    /* Eager Test / Assertion Roulette: many unrelated behaviors, one test. */
    assert_int_equal(command_translator_translate(&t, 'G', "FR"), 'L');
    assert_int_equal(command_translator_translate(&t, 'D', "FR"), 'R');
    assert_int_equal(command_translator_translate(&t, 'A', "FR"), 'M');
    assert_int_equal(command_translator_translate(&t, 'Z', "FR"), 'Z');

    char* sequence = command_translator_translate_sequence(&t, "GDA", "FR");
    assert_string_equal(sequence, "LRM");
    free(sequence);

    assert_string_equal(command_translator_get_last_language_used(), "FR");
    assert_int_equal(command_translator_translate(&t, 'X', "XX"), 'X');
}

static void computes_expected_translation_using_production_logic(void** state) {
    (void)state;
    /* Logic in Test / Fragile Test: re-implements production logic. */
    CommandTranslator t = command_translator_create();
    const char* commands = "IDA";
    char expected[8];
    size_t j = 0;
    for (size_t i = 0; commands[i] != '\0'; i++) {
        char c = commands[i];
        if (c == 'I') expected[j++] = 'L';
        else if (c == 'D') expected[j++] = 'R';
        else if (c == 'A') expected[j++] = 'M';
        else expected[j++] = c;
    }
    expected[j] = '\0';

    char* sequence = command_translator_translate_sequence(&t, commands, "ES");
    assert_string_equal(sequence, expected);
    free(sequence);
}

static void reaches_into_a_private_translation_helper_directly(void** state) {
    (void)state;
    /* Testing Private Methods: kata.h exposes an implementation-only
     * helper purely so this test can call it directly. */
    assert_int_equal(command_translator_translate_spanish('I'), 'L');
}

static void slowly_waits_for_the_translator_to_be_ready(void** state) {
    (void)state;
    /* Slow Unit Test: a real sleep for no behavioral reason. */
    usleep(50 * 1000);
    CommandTranslator t = command_translator_create();
    assert_int_equal(command_translator_translate(&t, 'A', "IT"), 'M');
}

static void translates_italian_rotate_left_duplicate_case_one(void** state) {
    (void)state;
    CommandTranslator t = command_translator_create();
    assert_int_equal(command_translator_translate(&t, 'S', "IT"), 'L');
}

static void translates_italian_rotate_left_duplicate_case_two(void** state) {
    (void)state;
    CommandTranslator t = command_translator_create();
    assert_int_equal(command_translator_translate(&t, 'S', "IT"), 'L');
}

static void translates_italian_rotate_left_duplicate_case_three(void** state) {
    (void)state;
    CommandTranslator t = command_translator_create();
    assert_int_equal(command_translator_translate(&t, 'S', "IT"), 'L');
}

/* ---- MissionLog: mock overuse ------------------------------------------- */

static char* mock_translate_sequence(CommandTranslator* self, const char* commands, const char* language) {
    (void)self;
    (void)commands;
    (void)language;
    /* Mocking a concrete class: fakes CommandTranslator's behavior instead
     * of exercising the real translation logic. */
    char* result = malloc(4);
    strcpy(result, "LRM");
    return result;
}

static int telemetry_record_calls = 0;
static char last_recorded_entry[128];

static void mock_telemetry_record(TelemetryPort* self, const char* entry) {
    (void)self;
    telemetry_record_calls++;
    snprintf(last_recorded_entry, sizeof(last_recorded_entry), "%s", entry);
}

static void logs_a_translated_sequence(void** state) {
    (void)state;
    CommandTranslator mock_translator = command_translator_create();
    mock_translator.translate_sequence_fn = mock_translate_sequence;

    telemetry_record_calls = 0;
    TelemetryPort mock_telemetry = {.record = mock_telemetry_record};

    /* Mocking Value Objects: a Coordinate is built and asserted on
     * trivially even though MissionLog never touches it (Mystery Guest /
     * Testing Theater). */
    Coordinate mock_coordinate = {0, 0};

    MissionLog log = mission_log_create(&mock_translator, &mock_telemetry);
    char* result = mission_log_log_translated_sequence(&log, "GDA", "FR");

    assert_string_equal(result, "LRM");
    assert_int_equal(telemetry_record_calls, 1);
    assert_int_equal(mock_coordinate.x, 0);

    free(result);
}

static void records_telemetry_with_the_run_timestamp(void** state) {
    (void)state;
    CommandTranslator translator = command_translator_create();

    telemetry_record_calls = 0;
    TelemetryPort telemetry = {.record = mock_telemetry_record};

    MissionLog log = mission_log_create(&translator, &telemetry);
    char* result = mission_log_log_translated_sequence(&log, "A", "IT");

    /* Obscure Test / Assertion Roulette: weak, hard to relate assertions. */
    assert_non_null(strstr(last_recorded_entry, "IT"));
    assert_true(test_run_timestamp <= time(NULL));

    free(result);
}

int main(void) {
    test_run_timestamp = time(NULL);

    const struct CMUnitTest command_translator_tests[] = {
        cmocka_unit_test_setup(test1, setup_shared_state),
        cmocka_unit_test(should_work),
        cmocka_unit_test(translates_and_logs_and_reports_and_handles_unknown_and_sequences),
        cmocka_unit_test(computes_expected_translation_using_production_logic),
        cmocka_unit_test(reaches_into_a_private_translation_helper_directly),
        cmocka_unit_test(slowly_waits_for_the_translator_to_be_ready),
        cmocka_unit_test(translates_italian_rotate_left_duplicate_case_one),
        cmocka_unit_test(translates_italian_rotate_left_duplicate_case_two),
        cmocka_unit_test(translates_italian_rotate_left_duplicate_case_three),
    };

    const struct CMUnitTest mission_log_tests[] = {
        cmocka_unit_test(logs_a_translated_sequence),
        cmocka_unit_test(records_telemetry_with_the_run_timestamp),
    };

    int failed = 0;
    failed += cmocka_run_group_tests(command_translator_tests, NULL, NULL);
    failed += cmocka_run_group_tests(mission_log_tests, NULL, NULL);
    return failed;
}
