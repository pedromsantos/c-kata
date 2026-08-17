#pragma once

/*
 * Legacy Code & Test Smells Kata - Shopping Cart (C port), Domain layer.
 *
 * No explanatory comments about *what is wrong* live in this header on
 * purpose -- finding and naming each testability blocker and test smell is
 * part of the exercise. See ../README.md.
 */

#include <stddef.h>

#define PRODUCT_CODE_MAX 16
#define PRODUCT_NAME_MAX 64
#define CART_ID_MAX 32
#define CUSTOMER_NAME_MAX 64
#define MAX_LINE_ITEMS 16

/* ---- Product -------------------------------------------------------------- */

typedef struct {
    char code[PRODUCT_CODE_MAX];
    char name[PRODUCT_NAME_MAX];
    double price;
} Product;

Product product_create(const char* code, const char* name, double price);
int product_equals(const Product* a, const Product* b);

/* ---- LineItem --------------------------------------------------------------- */

typedef struct {
    Product product;
    int quantity;
} LineItem;

/* ---- PromotionEngine ---------------------------------------------------------
 *
 * apply_fn is left NULL by promotion_engine_create() to use the real
 * pricing logic. It exists so a test can swap in a fake -- the C analogue
 * of "mocking a concrete class" for a struct that has no natural interface.
 * promotion_engine_price_for is exposed here (not kept file-static in
 * domain.c) purely so a test can reach into it directly.
 */

typedef struct PromotionEngine {
    double (*apply_fn)(struct PromotionEngine* self, const LineItem* items, int count);
} PromotionEngine;

PromotionEngine promotion_engine_create(void);
double promotion_engine_apply(PromotionEngine* engine, const LineItem* items, int count);
int promotion_engine_get_times_applied(void);
double promotion_engine_price_for(const LineItem* item);

/* ---- Cart -------------------------------------------------------------------- */

typedef struct {
    char id[CART_ID_MAX];
    char customer_name[CUSTOMER_NAME_MAX];
    LineItem items[MAX_LINE_ITEMS];
    int item_count;
    PromotionEngine promotion_engine;
} Cart;

Cart cart_create(const char* id, const char* customer_name);
void cart_add_product(Cart* cart, Product product, int quantity);
double cart_calculate_subtotal(Cart* cart);

/* ---- NotificationPort (Domain/Ports/NotificationPort.ts) --------------------- */

typedef struct NotificationPort {
    void (*send)(struct NotificationPort* self, const char* to, const char* message);
} NotificationPort;

/* ---- CartSummaryNotifier ------------------------------------------------------ */

typedef struct {
    PromotionEngine* promotion_engine;
    NotificationPort* notifications;
} CartSummaryNotifier;

CartSummaryNotifier cart_summary_notifier_create(PromotionEngine* engine, NotificationPort* notifications);
double cart_summary_notifier_notify_total(CartSummaryNotifier* notifier, const char* customer_email,
                                           const LineItem* items, int count);

/* ---- Clock (Domain/Ports/Clock.ts) --------------------------------------------- */

typedef struct Clock {
    /* Fills buffer (caller-owned) and returns it. */
    char* (*now)(struct Clock* self, char* buffer, size_t buffer_size);
} Clock;

/* ---- ShoppingCartRepository (Domain/Repositories/ShoppingCartRepository.ts) --- */

typedef struct ShoppingCartRepository {
    void (*save)(struct ShoppingCartRepository* self, const Cart* cart);
    /* Returns a pointer into repository-owned storage, or NULL. */
    Cart* (*find_by_id)(struct ShoppingCartRepository* self, const char* id);
} ShoppingCartRepository;
