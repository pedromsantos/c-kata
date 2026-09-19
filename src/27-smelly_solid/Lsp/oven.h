#pragma once

/*
 * SOLID Violations Kata - Liskov Substitution Principle, C port of
 * Lsp/Oven.ts, Lsp/GrillOven.ts, Lsp/Microwave.ts and Lsp/Chef.ts.
 *
 * LSP violation: Microwave can't honour Oven's cook() contract, so its
 * cook_fn always returns a nonzero "unsupported" code instead of actually
 * cooking -- callers that only know about Oven get a broken promise. The
 * kind-check in chef_cook() is the diagnostic signature of that violation:
 * a caller that can't just trust Oven's cook_fn.
 */

#define OVEN_KIND_GRILL 1
#define OVEN_KIND_MICROWAVE 2

typedef struct Oven {
    int kind;
    /* Returns 0 on success, nonzero if this Oven cannot cook(). */
    int (*cook_fn)(struct Oven* self, const char* food);
} Oven;

typedef struct {
    Oven base;
} GrillOven;

GrillOven grill_oven_create(void);

typedef struct {
    Oven base;
} Microwave;

Microwave microwave_create(void);
void microwave_cook_microwaving(Microwave* self, const char* food);

void chef_cook(Oven* oven, const char* food);
