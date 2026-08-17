# Legacy Code & Test Smells Kata - Yahtzee Dice

## Overview

This kata is a **fixture, not a from-scratch TDD exercise**. It is a small
dice-rolling slice of a Yahtzee game (`Die`, `DiceCup`, `TurnLog`), ported
from ts-kata's `17_SmellyYahtzee`. Unlike `21-smelly_mars_rover`, none of
the production code here is left completely untested -- every class has
tests. The catch is that the test suite is deliberately smelly: it passes,
but it does not reliably protect the behavior it claims to cover. It exists
to practice (or exercise tooling for):

- detecting and refactoring test smells in an existing, badly-tested suite
- making non-deterministic behavior (dice rolls) deterministic and testable
- characterizing rolling, selecting dice for reroll, and rerolling selected
  dice without weakening or deleting any existing test

Unlike `16-smelly_tic_tac_toe` (production code smells) and
`21-smelly_mars_rover` (testability blockers *and* test smells), this kata
bakes in **test smells only** -- the production code (`DiceCup`'s injectable
`RandomSource`, `TurnLog`'s injectable `TelemetryPort`) is already seam-friendly.

Neither `kata.h`/`kata.c` nor `test_kata.c` contain any explanatory comments
about *where* the problems are. Finding and naming each smell is part of the
exercise -- this README intentionally does not say where they are.

## Problem Description

A `DiceCup` holds 5 `Die`. `roll()` rolls all 5 using an injectable
`RandomSource` (defaults to a `rand()`-based source, mirroring ts-kata's
default `Math.random`). `selectForReroll`/`select_for_reroll` marks a subset
of dice indexes; `rerollSelected`/`reroll_selected` re-rolls just those and
clears the selection. `TurnLog` wraps a `DiceCup` and a `TelemetryPort`:
`rerollSelectedDice`/`turn_log_reroll_selected_dice` rerolls and records a
`"rerolled:v1,v2,..."` telemetry entry.

## What to Look For

The tests in `test_kata.c` currently all pass, but the suite is deliberately
smelly. It represents (at least) this subset of the smell catalogue:

Logic in Test, Mock Overuse, Test Interdependence, Fragile Test, Mystery
Guest, Eager Test, Obscure Test, Test Code Duplication, Hard-Coded Test
Data, Testing Private Methods, Slow Unit Test, Mocking Final/Concrete
Classes, Shared Mutable State, Testing Theater.

Some smells overlap in the same test. None of them are labeled in the code.

Note: C has no `private` keyword and no reflection-based "cast past the
access modifier" trick that the original TypeScript smelly test used. To
still demonstrate the "Testing Private Methods" smell, the C port exposes
`dice_cup_roll_die` directly in `kata.h`, even though it is an
implementation detail of `dice_cup_roll`/`dice_cup_reroll_selected`. That
exposure -- a header leaking an internal helper just so a test can reach it
-- is the idiomatic C shape of the same smell (same technique used by
`21-smelly_mars_rover`'s `command_translator_translate_spanish` and
friends). Likewise, `DiceCup.reroll_selected_fn` is a function-pointer field
added purely so a test can fake a "concrete class" instance -- the C
analogue of the TS test casting a plain object to `DiceCup`. See
`PORTING_NOTES_C.md` in `ts-kata-answers/17_SmellyYahtzee` for a C-specific
wrinkle in how the random-number seam had to be ported.

## Tasks

Fix the existing tests: everything in `test_kata.c` currently passes, but
several of them do not actually protect the behavior their name promises.
Detect each smell, then refactor it without ever weakening or deleting a
test to make it pass.

## Tips

- The smelly tests currently pass. That is the point: passing is not the
  same as protecting anything.
- Run mutation testing after any test fix -- high coverage on this file
  with weak assertions is exactly what several of the smells above look
  like from the outside.

## Run

```sh
cmake -B build
cmake --build build
./build/test_26-smelly_yahtzee
```
