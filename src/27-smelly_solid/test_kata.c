/*
 * Verification-fixture tests, ported from the TypeScript reference
 * (Srp/Car.ts, Ocp/CarEngineStatusReportController.ts, Lsp/Oven.ts +
 * GrillOven.ts + Microwave.ts + Chef.ts, Isp/IAmACar.ts + ElectricCar.ts,
 * Dip/MicrowaveGenerator.ts + MicrowaveOven.ts + Kitchen.ts). See README.md.
 * These tests exercise ordinary behaviour of each fixture -- they exist to
 * prove the fixtures build and run, not to hide the deliberate violation
 * (each file carries exactly one, documented in its own header/source
 * comment).
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <stdio.h>
#include <string.h>

#include "Dip/kitchen.h"
#include "Isp/electric_car.h"
#include "Lsp/oven.h"
#include "Ocp/car_engine_report.h"
#include "Srp/car.h"

/* ==== Srp/Car ================================================================= */

static void car_tracks_mileage_as_it_travels(void** state) {
    (void)state;
    Car car = car_create();
    assert_int_equal(car_current_mileage(&car), 0);

    Location destination = {10.0, 20.0};
    car_travel_to(&car, destination);

    assert_int_equal(car_current_mileage(&car), 1);
}

static void car_save_writes_its_state_to_disk(void** state) {
    (void)state;
    Car car = car_create();
    Location destination = {1.5, 2.5};
    car_travel_to(&car, destination);

    car_save(&car);

    FILE* file = fopen("/tmp/car.json", "r");
    assert_non_null(file);
    char buffer[256] = {0};
    size_t read = fread(buffer, 1, sizeof(buffer) - 1, file);
    (void)read;
    fclose(file);
    remove("/tmp/car.json");

    assert_non_null(strstr(buffer, "\"mileage\":1"));
}

/* ==== Ocp/CarEngineStatusReportController ===================================== */

static void displays_the_engine_status_as_html(void** state) {
    (void)state;
    CarEngineViewModel view_model = {4500, 90};
    char buffer[128];

    car_engine_status_report_controller_display(view_model, buffer, sizeof(buffer));

    assert_non_null(strstr(buffer, "4500"));
    assert_non_null(strstr(buffer, "<div>"));
}

static void prints_the_engine_status_as_text(void** state) {
    (void)state;
    CarEngineViewModel view_model = {4500, 90};
    char buffer[128];

    car_engine_status_report_controller_print(view_model, buffer, sizeof(buffer));

    assert_non_null(strstr(buffer, "RPM: 4500"));
}

/* ==== Lsp/Oven, GrillOven, Microwave, Chef ===================================== */

static void grill_oven_cooks_successfully(void** state) {
    (void)state;
    GrillOven grill = grill_oven_create();
    assert_int_equal(grill.base.cook_fn(&grill.base, "steak"), 0);
}

static void microwave_refuses_the_shared_cook_contract(void** state) {
    (void)state;
    Microwave microwave = microwave_create();
    assert_int_not_equal(microwave.base.cook_fn(&microwave.base, "popcorn"), 0);
}

static void chef_routes_microwaves_through_their_own_method(void** state) {
    (void)state;
    GrillOven grill = grill_oven_create();
    Microwave microwave = microwave_create();

    /* Neither call should fail: Chef special-cases Microwave via kind. */
    chef_cook(&grill.base, "steak");
    chef_cook(&microwave.base, "popcorn");
    assert_int_equal(microwave.base.kind, OVEN_KIND_MICROWAVE);
}

/* ==== Isp/IAmACar, ElectricCar ================================================== */

static void electric_car_drives_and_charges(void** state) {
    (void)state;
    ElectricCar car = electric_car_create();
    Location destination = {5.0, 6.0};

    car.base.go_to_fn(&car.base, destination);
    car.base.refill_electricity_fn(&car.base, 10.0);

    assert_int_equal(car.base.current_mileage_fn(&car.base), 1);
    assert_float_equal(car.battery_kilo_watts, 10.0, 0.0001);
}

static void electric_car_cannot_honour_the_gasoline_capability(void** state) {
    (void)state;
    ElectricCar car = electric_car_create();
    assert_int_not_equal(car.base.refill_gasoline_fn(&car.base, 5.0), 0);
}

/* ==== Dip/MicrowaveGenerator, MicrowaveOven, Kitchen ============================ */

static void kitchen_cooks_dinner_using_its_hardwired_oven(void** state) {
    (void)state;
    Kitchen kitchen = kitchen_create();
    /* Behaviourally unremarkable on purpose: the DIP violation is
     * structural (Kitchen can only ever be a MicrowaveOven kitchen), not
     * something a black-box assertion here can show. */
    kitchen_cook_dinner(&kitchen);
    assert_non_null(&kitchen);
}

int main(void) {
    const struct CMUnitTest srp_tests[] = {
        cmocka_unit_test(car_tracks_mileage_as_it_travels),
        cmocka_unit_test(car_save_writes_its_state_to_disk),
    };

    const struct CMUnitTest ocp_tests[] = {
        cmocka_unit_test(displays_the_engine_status_as_html),
        cmocka_unit_test(prints_the_engine_status_as_text),
    };

    const struct CMUnitTest lsp_tests[] = {
        cmocka_unit_test(grill_oven_cooks_successfully),
        cmocka_unit_test(microwave_refuses_the_shared_cook_contract),
        cmocka_unit_test(chef_routes_microwaves_through_their_own_method),
    };

    const struct CMUnitTest isp_tests[] = {
        cmocka_unit_test(electric_car_drives_and_charges),
        cmocka_unit_test(electric_car_cannot_honour_the_gasoline_capability),
    };

    const struct CMUnitTest dip_tests[] = {
        cmocka_unit_test(kitchen_cooks_dinner_using_its_hardwired_oven),
    };

    int failed = 0;
    failed += cmocka_run_group_tests(srp_tests, NULL, NULL);
    failed += cmocka_run_group_tests(ocp_tests, NULL, NULL);
    failed += cmocka_run_group_tests(lsp_tests, NULL, NULL);
    failed += cmocka_run_group_tests(isp_tests, NULL, NULL);
    failed += cmocka_run_group_tests(dip_tests, NULL, NULL);
    return failed;
}
