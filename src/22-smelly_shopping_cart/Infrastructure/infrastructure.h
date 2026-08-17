#pragma once

#include "../Domain/domain.h"

/* ---- OrderClock (Application/UseCases/Cart/OrderClock.ts) -------------------- */

typedef struct {
    Clock base;
} OrderClock;

OrderClock order_clock_create(void);
void order_clock_now_static(char* buffer, size_t buffer_size);

/* ---- EmailNotificationGateway -------------------------------------------------- */

#define EMAIL_FROM_ADDRESS_MAX 64

typedef struct {
    NotificationPort base;
    char from_address[EMAIL_FROM_ADDRESS_MAX];
} EmailNotificationGateway;

EmailNotificationGateway email_notification_gateway_create(const char* from_address);
EmailNotificationGateway email_notification_gateway_create_default(void);

/* ---- InMemoryShoppingCartRepository --------------------------------------------- */

#define MAX_CARTS 16

typedef struct {
    ShoppingCartRepository base;
} InMemoryShoppingCartRepository;

InMemoryShoppingCartRepository in_memory_shopping_cart_repository_create(void);
void in_memory_shopping_cart_repository_clear(void);
