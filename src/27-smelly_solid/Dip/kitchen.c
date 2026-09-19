#include "kitchen.h"

#include <stdio.h>

MicrowaveGenerator microwave_generator_create(void) {
    MicrowaveGenerator generator;
    generator.dummy = 0;
    return generator;
}

void microwave_generator_generate(const MicrowaveGenerator* self) {
    (void)self;
    printf("generating microwaves\n");
}

/* DIP violation: MicrowaveOven news up a concrete MicrowaveGenerator itself
 * instead of depending on an injected abstraction. */
MicrowaveOven microwave_oven_create(void) {
    MicrowaveOven oven;
    oven.heater = microwave_generator_create();
    return oven;
}

void microwave_oven_cook(const MicrowaveOven* self) {
    microwave_generator_generate(&self->heater);
}

/* DIP violation: Kitchen (high-level policy) directly constructs a concrete
 * MicrowaveOven (low-level detail) -- it can't work with any other kind of
 * oven. */
Kitchen kitchen_create(void) {
    Kitchen kitchen;
    kitchen.oven = microwave_oven_create();
    return kitchen;
}

void kitchen_cook_dinner(const Kitchen* self) {
    microwave_oven_cook(&self->oven);
}
