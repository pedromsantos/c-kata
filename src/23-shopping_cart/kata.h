#pragma once

/*
 * Shopping Cart Kata (DDD / hexagonal, C port).
 *
 * This is a starter scaffold, not a solution -- it ports the ts-kata stub
 * (an empty Cart model, a repository port, an in-memory stub adapter, and
 * three use-case port interfaces) at the same completion level. See
 * README.md for the product catalog and promotion rules to implement.
 *
 * Ports are structs of function pointers; adapters are structs that embed
 * the port struct as their first member and wire matching function
 * pointers into it (see 22-smelly_shopping_cart for a fuller worked
 * example of the same pattern).
 */

#include <stddef.h>

/* ---- Domain/Models/Cart.ts -------------------------------------------------
 *
 * Deliberately unimplemented, matching the empty Cart.ts in ts-kata. The
 * repository port below operates on an opaque `void*` handle for the same
 * reason the TS port types its methods as `object`: the aggregate has not
 * been designed yet.
 */

/* TODO: design the Cart aggregate (id, customer, line items, subtotal). */

/* ---- Domain/Repositories/ShoppingCartRepository.ts -------------------------- */

typedef struct ShoppingCartRepository {
    void (*save)(struct ShoppingCartRepository* self, void* cart);
    /* Returns NULL when not found. */
    void* (*get)(struct ShoppingCartRepository* self, const char* id);
    /* Fills *out_count with the number of carts and returns a
     * caller-owned array of opaque handles (may be NULL when empty). */
    void** (*get_all)(struct ShoppingCartRepository* self, size_t* out_count);
} ShoppingCartRepository;

/* ---- Infrastructure/Repositories/InMemoryShoppingCartRepository.ts --------- */

typedef struct {
    ShoppingCartRepository base;
} InMemoryShoppingCartRepository;

InMemoryShoppingCartRepository in_memory_shopping_cart_repository_create(void);

/* ---- Application/UseCases/Cart/AddProduct.ts -------------------------------- */

typedef struct AddProduct {
    void (*execute)(struct AddProduct* self, const char* cart_id, const char* product_id);
} AddProduct;

/* ---- Application/UseCases/Cart/CalculateCartPrice.ts ------------------------- */

typedef struct CalculateCartPrice {
    double (*query)(struct CalculateCartPrice* self, const char* cart_id);
} CalculateCartPrice;

/* ---- Application/UseCases/Cart/CreateEmpty.ts -------------------------------- */

typedef struct CreateEmpty {
    void (*execute)(struct CreateEmpty* self, const char* cart_id, const char* customer_name);
} CreateEmpty;
