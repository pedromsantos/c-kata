#include "kata.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Matches the ts-kata stub's `get(id) => Promise.resolve({ id })`: no real
 * storage yet, just a placeholder handle carrying the id back. */
typedef struct {
    char id[64];
} StubCartHandle;

static void in_memory_shopping_cart_repository_save(ShoppingCartRepository* self, void* cart) {
    (void)self;
    (void)cart;
    /* TODO: implement once the Cart aggregate exists. */
}

static void* in_memory_shopping_cart_repository_get(ShoppingCartRepository* self, const char* id) {
    (void)self;
    StubCartHandle* handle = malloc(sizeof(StubCartHandle));
    if (handle) {
        snprintf(handle->id, sizeof(handle->id), "%s", id);
    }
    return handle;
}

static void** in_memory_shopping_cart_repository_get_all(ShoppingCartRepository* self, size_t* out_count) {
    (void)self;
    *out_count = 0;
    return NULL;
}

InMemoryShoppingCartRepository in_memory_shopping_cart_repository_create(void) {
    InMemoryShoppingCartRepository repository;
    repository.base.save = in_memory_shopping_cart_repository_save;
    repository.base.get = in_memory_shopping_cart_repository_get;
    repository.base.get_all = in_memory_shopping_cart_repository_get_all;
    return repository;
}
