/*
 * Starter scaffold -- ts-kata ships this kata with no test files at all,
 * only the stub interfaces/adapter above. This file is intentionally
 * trivial: it wires the stub repository together so the build/test target
 * exists and is green, and leaves the real work as TODOs.
 *
 * See README.md for:
 * - the product catalog and promotion rules to implement on the Cart
 *   aggregate;
 * - the acceptance / unit / integration testing strategy expected once the
 *   aggregate and use cases are implemented.
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <stdlib.h>
#include <string.h>

#include "kata.h"

static void in_memory_repository_get_returns_a_handle_carrying_the_id(void** state) {
    (void)state;
    InMemoryShoppingCartRepository repository = in_memory_shopping_cart_repository_create();

    void* handle = repository.base.get(&repository.base, "cart-1");
    assert_non_null(handle);
    free(handle);
}

static void in_memory_repository_get_all_starts_empty(void** state) {
    (void)state;
    InMemoryShoppingCartRepository repository = in_memory_shopping_cart_repository_create();

    size_t count = 1;
    void** all = repository.base.get_all(&repository.base, &count);

    assert_int_equal(count, 0);
    assert_null(all);
}

/* TODO: design the Cart aggregate, then replace the smoke tests above with
 * real unit tests (Cart / promotion rules), integration tests
 * (InMemoryShoppingCartRepository once it actually stores something), and
 * acceptance tests (AddProduct / CalculateCartPrice / CreateEmpty wired
 * together end to end). */

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(in_memory_repository_get_returns_a_handle_carrying_the_id),
        cmocka_unit_test(in_memory_repository_get_all_starts_empty),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
