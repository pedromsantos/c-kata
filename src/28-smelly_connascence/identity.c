#include "identity.h"

typedef struct {
    int value;
} Counter;

/* Connascence of Identity: a single, shared, file-scope instance -- every
 * caller of the accessors below is coupled to this exact object existing
 * exactly once. */
static Counter global_counter = {0};

int global_counter_increment(void) {
    global_counter.value += 1;
    return global_counter.value;
}

int global_counter_current(void) {
    return global_counter.value;
}
