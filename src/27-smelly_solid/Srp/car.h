#pragma once

/*
 * SOLID Violations Kata - Single Responsibility Principle, C port of
 * Srp/Car.ts.
 *
 * SRP violation: Car mixes domain behaviour (mileage/travel) with a
 * persistence concern (save) -- two different reasons to change bundled
 * into one struct/file.
 */

typedef struct {
    double lat;
    double lng;
} Location;

typedef struct {
    int mileage;
    Location location;
} Car;

Car car_create(void);
int car_current_mileage(const Car* car);
void car_travel_to(Car* car, Location location);
void car_save(const Car* car);
