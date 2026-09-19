#include "oven.h"

#include <stdio.h>

static int grill_oven_cook(Oven* self, const char* food) {
    (void)self;
    printf("Grilling %s\n", food);
    return 0;
}

GrillOven grill_oven_create(void) {
    GrillOven oven;
    oven.base.kind = OVEN_KIND_GRILL;
    oven.base.cook_fn = grill_oven_cook;
    return oven;
}

/* LSP violation: Microwave cannot honour Oven's cook() contract -- it
 * refuses instead of cooking, unlike every other Oven. */
static int microwave_cook(Oven* self, const char* food) {
    (void)self;
    (void)food;
    return 1; /* unsupported: use microwave_cook_microwaving() instead */
}

Microwave microwave_create(void) {
    Microwave microwave;
    microwave.base.kind = OVEN_KIND_MICROWAVE;
    microwave.base.cook_fn = microwave_cook;
    return microwave;
}

void microwave_cook_microwaving(Microwave* self, const char* food) {
    (void)self;
    printf("Microwaving %s\n", food);
}

/* The kind-check here is client code reacting to the LSP violation above,
 * not itself the violation. */
void chef_cook(Oven* oven, const char* food) {
    if (oven->kind == OVEN_KIND_MICROWAVE) {
        microwave_cook_microwaving((Microwave*)oven, food);
    } else {
        oven->cook_fn(oven, food);
    }
}
