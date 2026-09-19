#pragma once

/*
 * Connascence Kata - Connascence of Identity, C port of the module-level
 * singleton example (Identity/counter.ts).
 *
 * Identity violation: global_counter is a single, file-scope shared
 * instance exposed only through these accessor functions -- every caller's
 * correctness depends on there being exactly one shared Counter, same as
 * the TS module-level `export const`. See counter_consumer.h/.c for the
 * consumer side of this dependency.
 */

int global_counter_increment(void);
int global_counter_current(void);
