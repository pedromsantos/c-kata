#include "infrastructure.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

/* ---- OrderClock -------------------------------------------------------------- */

void order_clock_now_static(char* buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm utc;
#if defined(_WIN32)
    gmtime_s(&utc, &now);
#else
    gmtime_r(&now, &utc);
#endif
    char stamp[24];
    strftime(stamp, sizeof(stamp), "%Y-%m-%dT%H:%M:%S", &utc);
    snprintf(buffer, buffer_size, "%s.000Z", stamp);
}

static char* order_clock_now_instance(Clock* self, char* buffer, size_t buffer_size) {
    (void)self;
    order_clock_now_static(buffer, buffer_size);
    return buffer;
}

OrderClock order_clock_create(void) {
    OrderClock clock_impl;
    clock_impl.base.now = order_clock_now_instance;
    return clock_impl;
}

/* ---- EmailNotificationGateway --------------------------------------------------- */

static void email_notification_gateway_send(NotificationPort* self, const char* to, const char* message) {
    EmailNotificationGateway* gateway = (EmailNotificationGateway*)self;
    printf("[EMAIL %s -> %s] %s\n", gateway->from_address, to, message);
}

EmailNotificationGateway email_notification_gateway_create(const char* from_address) {
    EmailNotificationGateway gateway;
    gateway.base.send = email_notification_gateway_send;
    snprintf(gateway.from_address, sizeof(gateway.from_address), "%s", from_address);
    return gateway;
}

EmailNotificationGateway email_notification_gateway_create_default(void) {
    return email_notification_gateway_create("orders@shop.example.com");
}

/* ---- InMemoryShoppingCartRepository ---------------------------------------------- */

static Cart carts_store[MAX_CARTS];
static int carts_count = 0;

static void in_memory_shopping_cart_repository_save(ShoppingCartRepository* self, const Cart* cart) {
    (void)self;
    for (int i = 0; i < carts_count; i++) {
        if (strcmp(carts_store[i].id, cart->id) == 0) {
            carts_store[i] = *cart;
            return;
        }
    }

    if (carts_count < MAX_CARTS) {
        carts_store[carts_count] = *cart;
        carts_count++;
    }
}

static Cart* in_memory_shopping_cart_repository_find_by_id(ShoppingCartRepository* self, const char* id) {
    (void)self;
    for (int i = 0; i < carts_count; i++) {
        if (strcmp(carts_store[i].id, id) == 0) {
            return &carts_store[i];
        }
    }
    return NULL;
}

InMemoryShoppingCartRepository in_memory_shopping_cart_repository_create(void) {
    InMemoryShoppingCartRepository repository;
    repository.base.save = in_memory_shopping_cart_repository_save;
    repository.base.find_by_id = in_memory_shopping_cart_repository_find_by_id;
    return repository;
}

void in_memory_shopping_cart_repository_clear(void) {
    carts_count = 0;
}
