#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_copy_list_of_characters(void **state) {
    (void) state; /* unused */
    Copier* character_copier = copier_create();
    assert_non_null(character_copier);
    copier_destroy(character_copier);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_copy_list_of_characters),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
