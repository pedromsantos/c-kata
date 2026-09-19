#pragma once

/*
 * SOLID Violations Kata - Interface Segregation Principle, C port of
 * Isp/IAmACar.ts and Isp/ElectricCar.ts.
 *
 * ISP violation: IAmACar bundles capabilities (gasoline refill included)
 * that no single implementer honestly supports all of -- visible directly
 * in the struct's own shape. ElectricCar is the forced implementer: its
 * refill_gasoline_fn always returns a nonzero "unsupported" code instead of
 * doing anything.
 */

#include "../Srp/car.h"

typedef struct IAmACar {
    void (*go_to_fn)(struct IAmACar* self, Location location);
    /* Returns 0 on success, nonzero if this car cannot take gasoline. */
    int (*refill_gasoline_fn)(struct IAmACar* self, double gallons);
    void (*refill_electricity_fn)(struct IAmACar* self, double kilo_watts);
    int (*current_mileage_fn)(struct IAmACar* self);
} IAmACar;

typedef struct {
    IAmACar base;
    int mileage;
    double battery_kilo_watts;
} ElectricCar;

ElectricCar electric_car_create(void);
