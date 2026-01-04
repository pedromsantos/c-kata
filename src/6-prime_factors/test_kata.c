#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_determine_prime_factor_1(void **state) {
    (void) state; /* unused */
    assert_int_equal(PRIME_FACTORS, 1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_determine_prime_factor_1),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
