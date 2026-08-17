#include "domain.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* ---- Product --------------------------------------------------------------- */

Product product_create(const char* code, const char* name, double price) {
    Product product;
    snprintf(product.code, sizeof(product.code), "%s", code);
    snprintf(product.name, sizeof(product.name), "%s", name);
    product.price = price;
    return product;
}

int product_equals(const Product* a, const Product* b) {
    return strcmp(a->code, b->code) == 0;
}

/* ---- PromotionEngine -------------------------------------------------------- */

static int times_applied = 0;

static const char* two_for_one_codes[] = {"VOUCHER"};
static const char* bulk_discount_code = "TSHIRT";
static const int bulk_discount_threshold = 3;
static const double bulk_discount_price = 19.0;

double promotion_engine_price_for(const LineItem* item) {
    for (size_t i = 0; i < sizeof(two_for_one_codes) / sizeof(two_for_one_codes[0]); i++) {
        if (strcmp(item->product.code, two_for_one_codes[i]) == 0) {
            double payable_units = ceil(item->quantity / 2.0);
            return payable_units * item->product.price;
        }
    }

    if (strcmp(item->product.code, bulk_discount_code) == 0 && item->quantity >= bulk_discount_threshold) {
        return item->quantity * bulk_discount_price;
    }

    return item->quantity * item->product.price;
}

static double promotion_engine_apply_default(PromotionEngine* self, const LineItem* items, int count) {
    (void)self;
    times_applied++;

    double total = 0;
    for (int i = 0; i < count; i++) {
        total += promotion_engine_price_for(&items[i]);
    }
    return total;
}

PromotionEngine promotion_engine_create(void) {
    PromotionEngine engine;
    engine.apply_fn = NULL;
    return engine;
}

double promotion_engine_apply(PromotionEngine* engine, const LineItem* items, int count) {
    if (engine->apply_fn) {
        return engine->apply_fn(engine, items, count);
    }
    return promotion_engine_apply_default(engine, items, count);
}

int promotion_engine_get_times_applied(void) {
    return times_applied;
}

/* ---- Cart -------------------------------------------------------------------- */

Cart cart_create(const char* id, const char* customer_name) {
    Cart cart;
    snprintf(cart.id, sizeof(cart.id), "%s", id);
    snprintf(cart.customer_name, sizeof(cart.customer_name), "%s", customer_name);
    cart.item_count = 0;
    cart.promotion_engine = promotion_engine_create();
    return cart;
}

void cart_add_product(Cart* cart, Product product, int quantity) {
    for (int i = 0; i < cart->item_count; i++) {
        if (product_equals(&cart->items[i].product, &product)) {
            cart->items[i].quantity += quantity;
            return;
        }
    }

    if (cart->item_count < MAX_LINE_ITEMS) {
        cart->items[cart->item_count].product = product;
        cart->items[cart->item_count].quantity = quantity;
        cart->item_count++;
    }
}

double cart_calculate_subtotal(Cart* cart) {
    return promotion_engine_apply(&cart->promotion_engine, cart->items, cart->item_count);
}

/* ---- CartSummaryNotifier ------------------------------------------------------ */

CartSummaryNotifier cart_summary_notifier_create(PromotionEngine* engine, NotificationPort* notifications) {
    CartSummaryNotifier notifier;
    notifier.promotion_engine = engine;
    notifier.notifications = notifications;
    return notifier;
}

double cart_summary_notifier_notify_total(CartSummaryNotifier* notifier, const char* customer_email,
                                           const LineItem* items, int count) {
    double total = promotion_engine_apply(notifier->promotion_engine, items, count);

    char message[128];
    snprintf(message, sizeof(message), "Cart total: %.2f\xE2\x82\xAC", total);
    notifier->notifications->send(notifier->notifications, customer_email, message);

    return total;
}
