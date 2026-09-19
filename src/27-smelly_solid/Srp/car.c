#include "car.h"

#include <stdio.h>

Car car_create(void) {
    Car car;
    car.mileage = 0;
    car.location.lat = 0;
    car.location.lng = 0;
    return car;
}

int car_current_mileage(const Car* car) {
    return car->mileage;
}

void car_travel_to(Car* car, Location location) {
    car->location = location;
    car->mileage += 1;
}

/* SRP violation: persistence concern living on the same struct/file as the
 * domain behaviour above -- see the header comment. */
void car_save(const Car* car) {
    FILE* file = fopen("/tmp/car.json", "w");
    if (file == NULL) {
        return;
    }
    fprintf(file, "{\"mileage\":%d,\"location\":{\"lat\":%f,\"lng\":%f}}", car->mileage, car->location.lat,
            car->location.lng);
    fclose(file);
}
