#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_leap_year(void **state) {
    (void) state; /* unused */
    assert_true(LEAP_YEAR);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_leap_year),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
