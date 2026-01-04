#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_tic_tac_toe_should_have_a_winner(void **state) {
    (void) state; /* unused */
    assert_string_equal(WINNER, "a player");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_tic_tac_toe_should_have_a_winner),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
