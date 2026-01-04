#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_rover_position(void **state) {
    (void) state; /* unused */
    assert_string_equal(MARS_ROVER, "Mars plateau");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_rover_position),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
