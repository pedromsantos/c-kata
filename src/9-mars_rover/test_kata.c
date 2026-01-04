#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_turn_360_degrees_clockwise(void **state) {
    (void) state; /* unused */
    const char* commands = "5 5\n1 2 N\nLMLMLMLMM";
    Rover* rover = rover_create();

    const char* position = rover_execute(rover, commands);

    assert_string_equal(position, commands);

    rover_destroy(rover);
}

static void test_turn_360_degrees_counter_clockwise(void **state) {
    (void) state; /* unused */
    const char* commands = "5 5\n3 3 E\nMMRMMRMRRM";
    Rover* rover = rover_create();

    const char* position = rover_execute(rover, commands);

    assert_string_equal(position, commands);

    rover_destroy(rover);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_turn_360_degrees_clockwise),
        cmocka_unit_test(test_turn_360_degrees_counter_clockwise),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
