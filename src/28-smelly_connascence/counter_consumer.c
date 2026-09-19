#include "counter_consumer.h"

#include "identity.h"

/* Connascence of Identity: this function's correctness depends on
 * global_counter_increment()/global_counter_current() sharing the exact
 * same underlying Counter instance -- there is no way to give this
 * consumer its own instance. */
int counter_consumer_bump_and_report(void) {
    global_counter_increment();
    return global_counter_current();
}
