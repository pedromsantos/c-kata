#include "application.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- ProductCatalog ------------------------------------------------------------ */

static const Product* product_catalog(void) {
    static Product catalog[3];
    static int initialized = 0;
    if (!initialized) {
        catalog[0] = product_create("VOUCHER", "Voucher", 5.0);
        catalog[1] = product_create("TSHIRT", "T-Shirt", 20.0);
        catalog[2] = product_create("MUG", "Coffee Mug", 7.5);
        initialized = 1;
    }
    return catalog;
}

int product_catalog_find(const char* code, Product* out) {
    const Product* catalog = product_catalog();
    for (int i = 0; i < 3; i++) {
        if (strcmp(catalog[i].code, code) == 0) {
            *out = catalog[i];
            return 1;
        }
    }
    fprintf(stderr, "Unknown product code %s\n", code);
    return 0;
}

/* ---- AddProductToCart ------------------------------------------------------------ */

AddProductToCart add_product_to_cart_create(ShoppingCartRepository* repository) {
    AddProductToCart use_case;
    use_case.repository = repository;
    return use_case;
}

int add_product_to_cart_execute(AddProductToCart* use_case, const char* cart_id, const char* product_code,
                                 int quantity) {
    Cart* cart = use_case->repository->find_by_id(use_case->repository, cart_id);
    if (!cart) {
        fprintf(stderr, "Cart %s not found\n", cart_id);
        return -1;
    }

    Product product;
    if (!product_catalog_find(product_code, &product)) {
        return -2;
    }

    cart_add_product(cart, product, quantity);
    use_case->repository->save(use_case->repository, cart);
    return 0;
}

/* ---- CheckoutCart ------------------------------------------------------------------ */

static double default_random_source(void) {
    return (double)rand() / ((double)RAND_MAX + 1.0);
}

CheckoutCart checkout_cart_create(ShoppingCartRepository* repository) {
    CheckoutCart use_case;
    use_case.repository = repository;
    use_case.default_notifier = email_notification_gateway_create_default();
    use_case.default_clock = order_clock_create();
    use_case.notifier = &use_case.default_notifier.base;
    use_case.clock = &use_case.default_clock.base;
    use_case.random_source = default_random_source;
    return use_case;
}

CheckoutCart checkout_cart_create_with(ShoppingCartRepository* repository, NotificationPort* notifier, Clock* clock,
                                        double (*random_source)(void)) {
    CheckoutCart use_case;
    use_case.repository = repository;
    use_case.notifier = notifier;
    use_case.clock = clock;
    use_case.random_source = random_source;
    return use_case;
}

int checkout_cart_execute(CheckoutCart* use_case, const char* cart_id, const char* customer_email,
                           Receipt* out_receipt) {
    Cart* cart = use_case->repository->find_by_id(use_case->repository, cart_id);
    if (!cart) {
        fprintf(stderr, "Cart %s not found\n", cart_id);
        return -1;
    }

    double total = cart_calculate_subtotal(cart);
    int code = (int)floor(use_case->random_source() * 1000000.0);

    snprintf(out_receipt->cart_id, sizeof(out_receipt->cart_id), "%s", cart_id);
    out_receipt->total = total;
    snprintf(out_receipt->confirmation_code, sizeof(out_receipt->confirmation_code), "ORD-%d", code);
    use_case->clock->now(use_case->clock, out_receipt->confirmed_at, sizeof(out_receipt->confirmed_at));

    char message[128];
    snprintf(message, sizeof(message), "Order confirmed: %s, total %.2f\xE2\x82\xAC", out_receipt->confirmation_code,
             total);
    use_case->notifier->send(use_case->notifier, customer_email, message);

    return 0;
}
