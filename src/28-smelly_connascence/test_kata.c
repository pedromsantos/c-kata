/*
 * Verification-fixture tests, ported from the TypeScript reference
 * (NotificationSystem.ts, TransportSelector.ts, ChecksumCalculator.ts,
 * ReceiptSender.ts, BackgroundJobRunner.ts, and the module-level Counter
 * singleton). See README.md. These tests exercise ordinary behaviour of
 * each fixture -- they exist to prove the fixtures build and run, not to
 * hide the deliberate violation (each file carries exactly one, documented
 * in its own header/source comment).
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <string.h>

#include "background_job_runner.h"
#include "checksum_calculator.h"
#include "counter_consumer.h"
#include "identity.h"
#include "notification_system.h"
#include "receipt_sender.h"
#include "transport_selector.h"

/* ==== NotificationSystem (Connascence of Position) ============================ */

static void sends_an_email_with_positional_arguments(void** state) {
    (void)state;
    /* No return value to assert on -- this exercises that the call
     * compiles and runs with the recipient/sender/message positions the
     * violation relies on. */
    notification_system_send_email("to@example.com", "from@example.com", "hello");
    assert_true(1);
}

/* ==== TransportSelector (Connascence of Meaning) =============================== */

static void resolves_magic_transport_codes_to_names(void** state) {
    (void)state;
    TransportSelector selector = transport_selector_create();

    transport_selector_set_transport(&selector, "1");
    assert_string_equal(selector.transport, "bike");

    transport_selector_set_transport(&selector, "3");
    assert_string_equal(selector.transport, "train");
}

/* ==== ChecksumCalculator (Connascence of Algorithm) ============================= */

static void adds_and_verifies_a_checksum_using_the_same_algorithm(void** state) {
    (void)state;
    char buffer[64];
    checksum_calculator_add_checksum("abc", buffer, sizeof(buffer));

    assert_true(checksum_calculator_check(buffer));
}

/* ==== ReceiptSender (Connascence of Execution Order) ============================ */

static void archiving_after_sending_is_the_correct_order(void** state) {
    (void)state;
    ReceiptSender sender = receipt_sender_create();

    receipt_sender_send_to_customer(&sender, "receipt-1");
    receipt_sender_archive(&sender, "receipt-1");

    assert_int_equal(sender.sent, 1);
}

static void archiving_before_sending_is_accepted_but_wrong(void** state) {
    (void)state;
    /* Demonstrates the violation: nothing prevents calling archive()
     * before send_to_customer() -- it just proceeds. */
    ReceiptSender sender = receipt_sender_create();

    receipt_sender_archive(&sender, "receipt-2");

    assert_int_equal(sender.sent, 0);
}

/* ==== BackgroundJobRunner (Connascence of Timing) ================================ */

static void waits_an_arbitrary_fixed_delay_for_the_result(void** state) {
    (void)state;
    BackgroundJobRunner runner = background_job_runner_create();
    background_job_runner_start_job(&runner);

    const char* result = background_job_runner_wait_for_result(&runner);

    assert_string_equal(result, "job-result");
}

/* ==== Counter singleton (Connascence of Identity) ================================ */

static void every_caller_shares_the_same_counter_instance(void** state) {
    (void)state;
    int before = global_counter_current();

    int reported = counter_consumer_bump_and_report();

    assert_int_equal(reported, before + 1);
    assert_int_equal(global_counter_current(), before + 1);
}

int main(void) {
    const struct CMUnitTest position_tests[] = {
        cmocka_unit_test(sends_an_email_with_positional_arguments),
    };

    const struct CMUnitTest meaning_tests[] = {
        cmocka_unit_test(resolves_magic_transport_codes_to_names),
    };

    const struct CMUnitTest algorithm_tests[] = {
        cmocka_unit_test(adds_and_verifies_a_checksum_using_the_same_algorithm),
    };

    const struct CMUnitTest execution_order_tests[] = {
        cmocka_unit_test(archiving_after_sending_is_the_correct_order),
        cmocka_unit_test(archiving_before_sending_is_accepted_but_wrong),
    };

    const struct CMUnitTest timing_tests[] = {
        cmocka_unit_test(waits_an_arbitrary_fixed_delay_for_the_result),
    };

    const struct CMUnitTest identity_tests[] = {
        cmocka_unit_test(every_caller_shares_the_same_counter_instance),
    };

    int failed = 0;
    failed += cmocka_run_group_tests(position_tests, NULL, NULL);
    failed += cmocka_run_group_tests(meaning_tests, NULL, NULL);
    failed += cmocka_run_group_tests(algorithm_tests, NULL, NULL);
    failed += cmocka_run_group_tests(execution_order_tests, NULL, NULL);
    failed += cmocka_run_group_tests(timing_tests, NULL, NULL);
    failed += cmocka_run_group_tests(identity_tests, NULL, NULL);
    return failed;
}
