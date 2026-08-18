/*
 * Deliberately smelly characterization tests, ported from the TypeScript
 * fixture (Tests/unit.test.ts, Tests/integration.test.ts,
 * Tests/product.unit.test.ts, Tests/repository.integration.test.ts,
 * Tests/cart-mother.unit.test.ts, Tests/acceptance.test.ts). See README.md
 * and ts-kata-answers/17_SmellyShoppingCart for background (not to be
 * copied wholesale -- this file matches the same "currently passes, but is
 * deliberately smelly" starting point).
 *
 * Note what is NOT deliberately tested here: the Application layer
 * (AddProductToCart) and the Cart aggregate's own behavior. That is
 * intentional -- see README.md.
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "Application/application.h"
#include "Domain/domain.h"
#include "Infrastructure/infrastructure.h"

/* ==== support/CartMother.ts ================================================= */

static Cart cart_mother_create(void) {
    return cart_create("cart-1", "Ada Lovelace");
}

static Cart cart_mother_empty_cart(void) {
    return cart_mother_create();
}

static Cart cart_mother_voucher_cart(int quantity) {
    Cart cart = cart_mother_create();
    cart_add_product(&cart, product_create("VOUCHER", "Voucher", 5), quantity);
    return cart;
}

static Cart cart_mother_tshirt_cart(int quantity) {
    Cart cart = cart_mother_create();
    cart_add_product(&cart, product_create("TSHIRT", "T-Shirt", 20), quantity);
    return cart;
}

/* ==== Tests/cart-mother.unit.test.ts ========================================= */

static void cart_mother_creates_a_valid_cart_with_stable_defaults(void** state) {
    (void)state;
    Cart cart = cart_mother_create();

    assert_string_equal(cart.id, "cart-1");
    assert_string_equal(cart.customer_name, "Ada Lovelace");
    assert_int_equal(cart.item_count, 0);
}

static void cart_mother_uses_named_scenarios(void** state) {
    (void)state;
    Cart empty_cart = cart_mother_empty_cart();
    Cart voucher_cart = cart_mother_voucher_cart(3);
    Cart tshirt_cart = cart_mother_tshirt_cart(4);

    assert_int_equal(empty_cart.item_count, 0);
    assert_int_equal(voucher_cart.item_count, 1);
    assert_string_equal(voucher_cart.items[0].product.code, "VOUCHER");
    assert_int_equal(voucher_cart.items[0].quantity, 3);
    assert_int_equal(tshirt_cart.item_count, 1);
    assert_string_equal(tshirt_cart.items[0].product.code, "TSHIRT");
    assert_int_equal(tshirt_cart.items[0].quantity, 4);
}

/* ==== Tests/product.unit.test.ts ============================================== */

static void treats_products_with_same_code_as_equal_despite_different_names(void** state) {
    (void)state;
    Product mug = product_create("MUG", "Coffee Mug", 7.5);
    Product other_mug = product_create("MUG", "Travel Mug", 12);
    assert_true(product_equals(&mug, &other_mug));
}

static void treats_products_with_same_code_as_equal_despite_different_prices(void** state) {
    (void)state;
    Product voucher = product_create("VOUCHER", "Gift Voucher", 5);
    Product other_voucher = product_create("VOUCHER", "Gift Voucher", 10);
    assert_true(product_equals(&voucher, &other_voucher));
}

static void treats_products_with_distinct_codes_as_different(void** state) {
    (void)state;
    Product mug = product_create("MUG", "Coffee Mug", 7.5);
    Product other_mug = product_create("MUG-PROMO", "Coffee Mug", 7.5);
    assert_false(product_equals(&mug, &other_mug));
}

/* ==== Tests/unit.test.ts: PromotionEngine (shared mutable state) ============= */

static PromotionEngine shared_engine;
static int run_count = 0;
static time_t test_run_timestamp;

static int setup_shared_engine(void** state) {
    (void)state;
    shared_engine = promotion_engine_create();
    return 0;
}

static void promotion_engine_test1(void** state) {
    (void)state;
    run_count++;
    LineItem items[] = {{product_create("VOUCHER", "Voucher", 5.0), 2}};
    double result = promotion_engine_apply(&shared_engine, items, 1);
    (void)result; /* Testing Theater: never asserted on */
    assert_true(1);
}

static void promotion_engine_should_work(void** state) {
    (void)state;
    /* Test Interdependence: relies on promotion_engine_test1() first. */
    assert_true(run_count > 0);
    assert_true(promotion_engine_get_times_applied() > 0);
}

static void prices_vouchers_tshirts_mugs_and_counts_applications(void** state) {
    (void)state;
    PromotionEngine engine = promotion_engine_create();
    Product voucher = product_create("VOUCHER", "Voucher", 5.0);
    Product tshirt = product_create("TSHIRT", "T-Shirt", 20.0);
    Product mug = product_create("MUG", "Coffee Mug", 7.5);

    LineItem two_vouchers[] = {{voucher, 2}};
    LineItem one_mug[] = {{mug, 1}};
    LineItem three_tshirts[] = {{tshirt, 3}};
    LineItem two_tshirts[] = {{tshirt, 2}};

    /* Eager Test / Assertion Roulette: many unrelated behaviors, one test. */
    assert_float_equal(promotion_engine_apply(&engine, two_vouchers, 1), 5.0, 0.0001);
    assert_float_equal(promotion_engine_apply(&engine, one_mug, 1), 7.5, 0.0001);
    assert_float_equal(promotion_engine_apply(&engine, three_tshirts, 1), 57.0, 0.0001);
    assert_float_equal(promotion_engine_apply(&engine, two_tshirts, 1), 40.0, 0.0001);
    assert_true(promotion_engine_get_times_applied() >= 4);
}

static void computes_expected_total_using_production_logic(void** state) {
    (void)state;
    /* Logic in Test / Fragile Test: re-implements production logic. */
    PromotionEngine engine = promotion_engine_create();
    LineItem items[] = {
        {product_create("VOUCHER", "Voucher", 5.0), 3},
        {product_create("TSHIRT", "T-Shirt", 20.0), 4},
    };

    double expected = 0;
    for (int i = 0; i < 2; i++) {
        LineItem* item = &items[i];
        if (strcmp(item->product.code, "VOUCHER") == 0) {
            expected += ceil(item->quantity / 2.0) * item->product.price;
        } else if (strcmp(item->product.code, "TSHIRT") == 0 && item->quantity >= 3) {
            expected += item->quantity * 19.0;
        } else {
            expected += item->quantity * item->product.price;
        }
    }

    assert_float_equal(promotion_engine_apply(&engine, items, 2), expected, 0.0001);
}

static void reaches_into_a_private_pricing_helper_directly(void** state) {
    (void)state;
    /* Testing Private Methods: domain.h exposes an implementation-only
     * helper purely so this test can call it directly. */
    LineItem item = {product_create("MUG", "Coffee Mug", 7.5), 1};
    assert_float_equal(promotion_engine_price_for(&item), 7.5, 0.0001);
}

static void slowly_waits_for_the_engine_to_be_ready(void** state) {
    (void)state;
    /* Slow Unit Test: a real sleep for no behavioral reason. */
    usleep(50 * 1000);
    PromotionEngine engine = promotion_engine_create();
    LineItem item[] = {{product_create("MUG", "Coffee Mug", 7.5), 1}};
    assert_float_equal(promotion_engine_apply(&engine, item, 1), 7.5, 0.0001);
}

static void prices_a_single_mug_duplicate_case_one(void** state) {
    (void)state;
    PromotionEngine engine = promotion_engine_create();
    LineItem item[] = {{product_create("MUG", "Coffee Mug", 7.5), 1}};
    assert_float_equal(promotion_engine_apply(&engine, item, 1), 7.5, 0.0001);
}

static void prices_a_single_mug_duplicate_case_two(void** state) {
    (void)state;
    PromotionEngine engine = promotion_engine_create();
    LineItem item[] = {{product_create("MUG", "Coffee Mug", 7.5), 1}};
    assert_float_equal(promotion_engine_apply(&engine, item, 1), 7.5, 0.0001);
}

static void prices_a_single_mug_duplicate_case_three(void** state) {
    (void)state;
    PromotionEngine engine = promotion_engine_create();
    LineItem item[] = {{product_create("MUG", "Coffee Mug", 7.5), 1}};
    assert_float_equal(promotion_engine_apply(&engine, item, 1), 7.5, 0.0001);
}

/* ==== Tests/unit.test.ts: CartSummaryNotifier (mock overuse) ================= */

static double mock_apply(PromotionEngine* self, const LineItem* items, int count) {
    (void)self;
    (void)items;
    (void)count;
    return 42.0;
}

static int notification_send_calls = 0;
static char last_notification_message[128];

static void mock_notification_send(NotificationPort* self, const char* to, const char* message) {
    (void)self;
    (void)to;
    notification_send_calls++;
    snprintf(last_notification_message, sizeof(last_notification_message), "%s", message);
}

static void notifies_the_customer_of_the_cart_total(void** state) {
    (void)state;
    PromotionEngine mock_engine = promotion_engine_create();
    mock_engine.apply_fn = mock_apply;

    notification_send_calls = 0;
    NotificationPort mock_notifications = {.send = mock_notification_send};

    /* Mocking Value Objects: a Product test double is built and asserted
     * on trivially (Mystery Guest / Testing Theater). */
    Product mock_product = product_create("MUG", "Coffee Mug", 7.5);
    LineItem items[] = {{mock_product, 1}};

    CartSummaryNotifier notifier = cart_summary_notifier_create(&mock_engine, &mock_notifications);
    double total = cart_summary_notifier_notify_total(&notifier, "customer@example.com", items, 1);

    assert_float_equal(total, 42.0, 0.0001);
    assert_int_equal(notification_send_calls, 1);
    assert_string_equal(mock_product.code, "MUG");
}

static void records_the_run_timestamp_alongside_the_notification(void** state) {
    (void)state;
    PromotionEngine engine = promotion_engine_create();
    notification_send_calls = 0;
    NotificationPort notifications = {.send = mock_notification_send};

    CartSummaryNotifier notifier = cart_summary_notifier_create(&engine, &notifications);
    LineItem items[] = {{product_create("MUG", "Coffee Mug", 7.5), 1}};

    cart_summary_notifier_notify_total(&notifier, "customer@example.com", items, 1);

    /* Obscure Test / Assertion Roulette: weak, hard to relate assertions. */
    assert_non_null(strstr(last_notification_message, "Cart total"));
    assert_true(test_run_timestamp <= time(NULL));
}

/* ==== Tests/integration.test.ts: InMemoryShoppingCartRepository ============== */

static void repository_test2(void** state) {
    (void)state;
    InMemoryShoppingCartRepository repository = in_memory_shopping_cart_repository_create();
    Cart cart = cart_create("cart-1", "Ada Lovelace");
    cart_add_product(&cart, product_create("MUG", "Coffee Mug", 7.5), 1);

    repository.base.save(&repository.base, &cart);

    assert_true(1); /* Testing Theater: never really asserts on the save. */
}

static void repository_finds_the_cart_saved_earlier(void** state) {
    (void)state;
    /* Test Interdependence: relies on repository_test2() having saved
     * "cart-1" into the shared static store first. */
    InMemoryShoppingCartRepository repository = in_memory_shopping_cart_repository_create();
    Cart* found = repository.base.find_by_id(&repository.base, "cart-1");
    assert_non_null(found);
}

static void saves_refinds_mutates_resaves_and_counts_items(void** state) {
    (void)state;
    InMemoryShoppingCartRepository repository = in_memory_shopping_cart_repository_create();
    Cart cart = cart_create("cart-2", "Grace Hopper");
    cart_add_product(&cart, product_create("VOUCHER", "Voucher", 5.0), 1);
    repository.base.save(&repository.base, &cart);

    Cart* first_find = repository.base.find_by_id(&repository.base, "cart-2");
    cart_add_product(first_find, product_create("TSHIRT", "T-Shirt", 20.0), 1);
    repository.base.save(&repository.base, first_find);

    Cart* second_find = repository.base.find_by_id(&repository.base, "cart-2");
    assert_non_null(second_find);
    assert_string_equal(second_find->id, "cart-2");
    assert_string_equal(second_find->customer_name, "Grace Hopper");
    assert_int_equal(second_find->item_count, 2);
    assert_null(repository.base.find_by_id(&repository.base, "does-not-exist"));
}

static void slowly_waits_for_the_in_memory_store_to_be_ready(void** state) {
    (void)state;
    /* Slow Unit Test: a real sleep for no behavioral reason. */
    usleep(50 * 1000);
    InMemoryShoppingCartRepository repository = in_memory_shopping_cart_repository_create();
    Cart cart = cart_create("cart-3", "Margaret Hamilton");
    repository.base.save(&repository.base, &cart);
    assert_non_null(repository.base.find_by_id(&repository.base, "cart-3"));
}

static void saves_a_cart_double_instead_of_a_real_cart(void** state) {
    (void)state;
    /* Mocking Final/Concrete Classes: the TS original stores an object
     * literal cast to Cart, then asserts on reference identity. C's value
     * semantics make that particular assertion meaningless (save() always
     * copies) -- see PORTING_NOTES_C.md. This keeps the spirit by using a
     * hand-built "double" instead of the real cart_create constructor and
     * asserting on its content, not on pointer identity. */
    Cart mock_cart;
    snprintf(mock_cart.id, sizeof(mock_cart.id), "cart-4");
    snprintf(mock_cart.customer_name, sizeof(mock_cart.customer_name), "Katherine Johnson");
    mock_cart.item_count = 0;
    mock_cart.promotion_engine = promotion_engine_create();

    InMemoryShoppingCartRepository repository = in_memory_shopping_cart_repository_create();
    repository.base.save(&repository.base, &mock_cart);
    Cart* found = repository.base.find_by_id(&repository.base, "cart-4");

    assert_non_null(found);
    assert_string_equal(found->customer_name, "Katherine Johnson");
}

/* ==== Tests/integration.test.ts: EmailNotificationGateway ==================== */

static void sends_an_order_confirmation_email(void** state) {
    (void)state;
    /* Port-Boundary Violation: asserts against stdout instead of behavior
     * behind a seam -- kept as-is, matching the TS console.log spy smell,
     * just redirected to a temp file instead of a jest spy. */
    char path[] = "/tmp/c_kata_smelly_cart_email_XXXXXX";
    int fd = mkstemp(path);
    assert_true(fd != -1);

    FILE* original_stdout = stdout;
    FILE* redirected = fdopen(fd, "w");
    stdout = redirected;

    EmailNotificationGateway gateway = email_notification_gateway_create_default();
    gateway.base.send(&gateway.base, "customer@example.com", "Order confirmed: ORD-1");

    fflush(redirected);
    stdout = original_stdout;
    fclose(redirected);

    FILE* readback = fopen(path, "r");
    char buffer[256] = {0};
    size_t read = fread(buffer, 1, sizeof(buffer) - 1, readback);
    (void)read;
    fclose(readback);
    remove(path);

    assert_non_null(strstr(buffer, "customer@example.com"));
}

/* ==== Tests/repository.integration.test.ts ==================================== */

static Cart a_cart_with_products(const char* id) {
    Cart cart = cart_create(id, "Ada Lovelace");
    cart_add_product(&cart, product_create("MUG", "Coffee Mug", 7.5), 2);
    cart_add_product(&cart, product_create("VOUCHER", "Gift Voucher", 5), 1);
    return cart;
}

static int setup_clear_repository(void** state) {
    (void)state;
    in_memory_shopping_cart_repository_clear();
    return 0;
}

static int teardown_clear_repository(void** state) {
    (void)state;
    in_memory_shopping_cart_repository_clear();
    return 0;
}

static void finds_cart_when_saved_through_repository(void** state) {
    (void)state;
    InMemoryShoppingCartRepository repository = in_memory_shopping_cart_repository_create();
    Cart cart = a_cart_with_products("repository-integration-cart-1");

    repository.base.save(&repository.base, &cart);
    Cart* found = repository.base.find_by_id(&repository.base, cart.id);

    assert_non_null(found);
    assert_string_equal(found->id, cart.id);
    assert_int_equal(found->item_count, cart.item_count);
}

static void returns_null_when_cart_id_is_unknown(void** state) {
    (void)state;
    InMemoryShoppingCartRepository repository = in_memory_shopping_cart_repository_create();
    assert_null(repository.base.find_by_id(&repository.base, "unknown-cart"));
}

/* ==== Tests/acceptance.test.ts: CheckoutCart =================================== */

static char fake_repo_ids[MAX_CARTS][CART_ID_MAX];
static Cart fake_repo_carts[MAX_CARTS];
static int fake_repo_count;

static int fake_repo_setup(void** state) {
    (void)state;
    fake_repo_count = 0;
    return 0;
}

static void fake_repo_seed(Cart cart) {
    snprintf(fake_repo_ids[fake_repo_count], CART_ID_MAX, "%s", cart.id);
    fake_repo_carts[fake_repo_count] = cart;
    fake_repo_count++;
}

static void fake_repo_save(ShoppingCartRepository* self, const Cart* cart) {
    (void)self;
    for (int i = 0; i < fake_repo_count; i++) {
        if (strcmp(fake_repo_ids[i], cart->id) == 0) {
            fake_repo_carts[i] = *cart;
            return;
        }
    }
    fake_repo_seed(*cart);
}

static Cart* fake_repo_find_by_id(ShoppingCartRepository* self, const char* id) {
    (void)self;
    for (int i = 0; i < fake_repo_count; i++) {
        if (strcmp(fake_repo_ids[i], id) == 0) {
            return &fake_repo_carts[i];
        }
    }
    return NULL;
}

static char* fixed_clock_now(Clock* self, char* buffer, size_t buffer_size) {
    (void)self;
    snprintf(buffer, buffer_size, "2024-01-01T00:00:00.000Z");
    return buffer;
}

static double fixed_random_source(void) {
    return 0.5; /* -> floor(0.5 * 1_000_000) = 500000 */
}

static void confirms_checkout_with_no_discounts(void** state) {
    (void)state;
    ShoppingCartRepository repository = {.save = fake_repo_save, .find_by_id = fake_repo_find_by_id};
    notification_send_calls = 0;
    NotificationPort notifier = {.send = mock_notification_send};
    Clock clock = {.now = fixed_clock_now};

    Cart cart = cart_create("cart-1", "Ada Lovelace");
    cart_add_product(&cart, product_create("MUG", "Coffee Mug", 7.5), 1);
    fake_repo_seed(cart);

    CheckoutCart use_case = checkout_cart_create_with(&repository, &notifier, &clock, fixed_random_source);
    Receipt receipt;
    int status = checkout_cart_execute(&use_case, "cart-1", "ada@example.com", &receipt);

    assert_int_equal(status, 0);
    assert_string_equal(receipt.cart_id, "cart-1");
    assert_float_equal(receipt.total, 7.5, 0.0001);
    assert_string_equal(receipt.confirmation_code, "ORD-500000");
    assert_string_equal(receipt.confirmed_at, "2024-01-01T00:00:00.000Z");
}

static void notifies_the_customer_of_the_confirmed_total(void** state) {
    (void)state;
    ShoppingCartRepository repository = {.save = fake_repo_save, .find_by_id = fake_repo_find_by_id};
    notification_send_calls = 0;
    NotificationPort notifier = {.send = mock_notification_send};
    Clock clock = {.now = fixed_clock_now};

    Cart cart = cart_create("cart-2", "Ada Lovelace");
    cart_add_product(&cart, product_create("MUG", "Coffee Mug", 7.5), 1);
    fake_repo_seed(cart);

    CheckoutCart use_case = checkout_cart_create_with(&repository, &notifier, &clock, fixed_random_source);
    Receipt receipt;
    checkout_cart_execute(&use_case, "cart-2", "ada@example.com", &receipt);

    assert_int_equal(notification_send_calls, 1);
    char expected[128];
    snprintf(expected, sizeof(expected), "Order confirmed: %s, total 7.50\xE2\x82\xAC", receipt.confirmation_code);
    assert_string_equal(last_notification_message, expected);
}

static void computes_total_with_real_promotion_rules(void** state) {
    (void)state;
    ShoppingCartRepository repository = {.save = fake_repo_save, .find_by_id = fake_repo_find_by_id};
    notification_send_calls = 0;
    NotificationPort notifier = {.send = mock_notification_send};
    Clock clock = {.now = fixed_clock_now};

    Cart cart = cart_create("cart-3", "Grace Hopper");
    cart_add_product(&cart, product_create("VOUCHER", "Voucher", 5.0), 3); /* 2 payable * 5.0 = 10.0 */
    fake_repo_seed(cart);

    CheckoutCart use_case = checkout_cart_create_with(&repository, &notifier, &clock, fixed_random_source);
    Receipt receipt;
    checkout_cart_execute(&use_case, "cart-3", "grace@example.com", &receipt);

    assert_float_equal(receipt.total, 10.0, 0.0001);
}

static void rejects_checkout_when_the_cart_does_not_exist(void** state) {
    (void)state;
    ShoppingCartRepository repository = {.save = fake_repo_save, .find_by_id = fake_repo_find_by_id};
    notification_send_calls = 0;
    NotificationPort notifier = {.send = mock_notification_send};
    Clock clock = {.now = fixed_clock_now};

    CheckoutCart use_case = checkout_cart_create_with(&repository, &notifier, &clock, fixed_random_source);
    Receipt receipt;
    int status = checkout_cart_execute(&use_case, "missing-cart", "nobody@example.com", &receipt);

    assert_int_equal(status, -1);
    assert_int_equal(notification_send_calls, 0);
}

int main(void) {
    test_run_timestamp = time(NULL);

    const struct CMUnitTest cart_mother_tests[] = {
        cmocka_unit_test(cart_mother_creates_a_valid_cart_with_stable_defaults),
        cmocka_unit_test(cart_mother_uses_named_scenarios),
    };

    const struct CMUnitTest product_tests[] = {
        cmocka_unit_test(treats_products_with_same_code_as_equal_despite_different_names),
        cmocka_unit_test(treats_products_with_same_code_as_equal_despite_different_prices),
        cmocka_unit_test(treats_products_with_distinct_codes_as_different),
    };

    const struct CMUnitTest promotion_engine_tests[] = {
        cmocka_unit_test_setup(promotion_engine_test1, setup_shared_engine),
        cmocka_unit_test(promotion_engine_should_work),
        cmocka_unit_test(prices_vouchers_tshirts_mugs_and_counts_applications),
        cmocka_unit_test(computes_expected_total_using_production_logic),
        cmocka_unit_test(reaches_into_a_private_pricing_helper_directly),
        cmocka_unit_test(slowly_waits_for_the_engine_to_be_ready),
        cmocka_unit_test(prices_a_single_mug_duplicate_case_one),
        cmocka_unit_test(prices_a_single_mug_duplicate_case_two),
        cmocka_unit_test(prices_a_single_mug_duplicate_case_three),
    };

    const struct CMUnitTest cart_summary_notifier_tests[] = {
        cmocka_unit_test(notifies_the_customer_of_the_cart_total),
        cmocka_unit_test(records_the_run_timestamp_alongside_the_notification),
    };

    const struct CMUnitTest repository_tests[] = {
        cmocka_unit_test(repository_test2),
        cmocka_unit_test(repository_finds_the_cart_saved_earlier),
        cmocka_unit_test(saves_refinds_mutates_resaves_and_counts_items),
        cmocka_unit_test(slowly_waits_for_the_in_memory_store_to_be_ready),
        cmocka_unit_test(saves_a_cart_double_instead_of_a_real_cart),
    };

    const struct CMUnitTest gateway_tests[] = {
        cmocka_unit_test(sends_an_order_confirmation_email),
    };

    const struct CMUnitTest repository_integration_tests[] = {
        cmocka_unit_test_setup_teardown(finds_cart_when_saved_through_repository, setup_clear_repository,
                                         teardown_clear_repository),
        cmocka_unit_test_setup_teardown(returns_null_when_cart_id_is_unknown, setup_clear_repository,
                                         teardown_clear_repository),
    };

    const struct CMUnitTest checkout_cart_tests[] = {
        cmocka_unit_test_setup(confirms_checkout_with_no_discounts, fake_repo_setup),
        cmocka_unit_test_setup(notifies_the_customer_of_the_confirmed_total, fake_repo_setup),
        cmocka_unit_test_setup(computes_total_with_real_promotion_rules, fake_repo_setup),
        cmocka_unit_test_setup(rejects_checkout_when_the_cart_does_not_exist, fake_repo_setup),
    };

    int failed = 0;
    failed += cmocka_run_group_tests(cart_mother_tests, NULL, NULL);
    failed += cmocka_run_group_tests(product_tests, NULL, NULL);
    failed += cmocka_run_group_tests(promotion_engine_tests, NULL, NULL);
    failed += cmocka_run_group_tests(cart_summary_notifier_tests, NULL, NULL);
    failed += cmocka_run_group_tests(repository_tests, NULL, NULL);
    failed += cmocka_run_group_tests(gateway_tests, NULL, NULL);
    failed += cmocka_run_group_tests(repository_integration_tests, NULL, NULL);
    failed += cmocka_run_group_tests(checkout_cart_tests, NULL, NULL);
    return failed;
}
