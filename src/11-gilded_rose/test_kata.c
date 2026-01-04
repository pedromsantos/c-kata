#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kata.h"

static void test_foo(void **state) {
    (void) state; /* unused */
    Item* items = item_create("foo", 0, 0);
    GildedRose* app = gilded_rose_create(items, 1);

    gilded_rose_update_quality(app);

    assert_string_equal(items->name, "foo");

    gilded_rose_destroy(app);
    item_destroy(items);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_foo),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
