#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_use_case_creation(void **state) {
    (void) state; /* unused */
    UseCase* use_case = use_case_create();
    assert_non_null(use_case);
    use_case_destroy(use_case);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_use_case_creation),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
