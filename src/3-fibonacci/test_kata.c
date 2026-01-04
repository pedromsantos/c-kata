#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_fibonacci_starts_at_zero(void **state) {
    (void) state; /* unused */
    assert_int_equal(FIBONACCI, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_fibonacci_starts_at_zero),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
