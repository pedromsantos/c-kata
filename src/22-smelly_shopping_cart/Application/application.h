#pragma once

#include "../Domain/domain.h"
#include "../Infrastructure/infrastructure.h"

/* ---- ProductCatalog (a private static-ish helper, hardcoded inside
 * AddProductToCart.ts in the original -- kept file-static in application.c
 * on purpose, only product_catalog_find is exposed). ------------------------ */

/* Returns 1 and fills *out when found, 0 (and leaves *out untouched) when
 * the product code is unknown. */
int product_catalog_find(const char* code, Product* out);

/* ---- AddProductToCart --------------------------------------------------------- */

typedef struct {
    ShoppingCartRepository* repository;
} AddProductToCart;

AddProductToCart add_product_to_cart_create(ShoppingCartRepository* repository);

/* Returns 0 on success, -1 when the cart is not found, -2 when the product
 * code is unknown (mirrors the thrown Errors in the TS use case). */
int add_product_to_cart_execute(AddProductToCart* use_case, const char* cart_id, const char* product_code,
                                 int quantity);

/* ---- CheckoutCart --------------------------------------------------------------- */

typedef struct {
    char cart_id[CART_ID_MAX];
    double total;
    char confirmation_code[32];
    char confirmed_at[32];
} Receipt;

typedef struct {
    ShoppingCartRepository* repository;
    NotificationPort* notifier;
    Clock* clock;
    double (*random_source)(void);

    /* Default dependencies, constructed eagerly by checkout_cart_create --
     * the C shape of `notifier = new EmailNotificationGateway()` as a
     * constructor default parameter. */
    EmailNotificationGateway default_notifier;
    OrderClock default_clock;
} CheckoutCart;

/* Wires the production defaults (EmailNotificationGateway, OrderClock,
 * rand()-based randomness) -- same testability blocker as the TS default
 * constructor parameters. */
CheckoutCart checkout_cart_create(ShoppingCartRepository* repository);

/* Escape hatch used by tests to inject fakes for every collaborator. */
CheckoutCart checkout_cart_create_with(ShoppingCartRepository* repository, NotificationPort* notifier, Clock* clock,
                                        double (*random_source)(void));

/* Returns 0 on success, -1 when the cart is not found. */
int checkout_cart_execute(CheckoutCart* use_case, const char* cart_id, const char* customer_email,
                           Receipt* out_receipt);
