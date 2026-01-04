#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_stack_should_be_zero(void **state) {
    (void) state; /* unused */
    assert_int_equal(STACK, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_stack_should_be_zero),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
