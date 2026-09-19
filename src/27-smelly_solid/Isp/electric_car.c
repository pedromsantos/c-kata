#include "electric_car.h"

#include <stdio.h>

static void electric_car_go_to(IAmACar* self, Location location) {
    ElectricCar* car = (ElectricCar*)self;
    car->mileage += 1;
    printf("Driving to %f, %f\n", location.lat, location.lng);
}

/* ISP violation: ElectricCar is forced to implement a capability it does
 * not support because IAmACar bundles it in -- see the header comment. */
static int electric_car_refill_gasoline(IAmACar* self, double gallons) {
    (void)self;
    (void)gallons;
    return 1; /* unsupported: electric cars don't take gasoline */
}

static void electric_car_refill_electricity(IAmACar* self, double kilo_watts) {
    ElectricCar* car = (ElectricCar*)self;
    car->battery_kilo_watts += kilo_watts;
}

static int electric_car_current_mileage(IAmACar* self) {
    ElectricCar* car = (ElectricCar*)self;
    return car->mileage;
}

ElectricCar electric_car_create(void) {
    ElectricCar car;
    car.base.go_to_fn = electric_car_go_to;
    car.base.refill_gasoline_fn = electric_car_refill_gasoline;
    car.base.refill_electricity_fn = electric_car_refill_electricity;
    car.base.current_mileage_fn = electric_car_current_mileage;
    car.mileage = 0;
    car.battery_kilo_watts = 0;
    return car;
}
