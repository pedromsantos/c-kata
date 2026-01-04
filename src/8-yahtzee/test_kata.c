#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_yahtzee_initial_score(void **state) {
    (void) state; /* unused */
    assert_int_equal(YAHTZEE, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_yahtzee_initial_score),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
