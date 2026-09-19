#pragma once

/*
 * SOLID Violations Kata - Dependency Inversion Principle, C port of
 * Dip/MicrowaveGenerator.ts, Dip/MicrowaveOven.ts and Dip/Kitchen.ts.
 *
 * DIP violation: MicrowaveOven's "constructor" builds a concrete
 * MicrowaveGenerator itself instead of depending on an injected
 * abstraction, and Kitchen's "constructor" builds a concrete MicrowaveOven
 * itself instead of depending on an injected abstraction -- Kitchen
 * (high-level policy) can't work with any other kind of oven.
 */

typedef struct {
    int dummy;
} MicrowaveGenerator;

MicrowaveGenerator microwave_generator_create(void);
void microwave_generator_generate(const MicrowaveGenerator* self);

typedef struct {
    MicrowaveGenerator heater;
} MicrowaveOven;

MicrowaveOven microwave_oven_create(void);
void microwave_oven_cook(const MicrowaveOven* self);

typedef struct {
    MicrowaveOven oven;
} Kitchen;

Kitchen kitchen_create(void);
void kitchen_cook_dinner(const Kitchen* self);
