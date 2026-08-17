# Legacy Code & Test Smells Kata - Mars Rover

## Overview

This kata is a **fixture, not a from-scratch TDD exercise**. It is a Mars
Rover mission simulator (inspired by `11-mars_rover` and `18-esa_mars_rover`)
built deliberately as legacy code: some of it has **zero tests**, and the
part that does have tests has **deliberately smelly ones**. It exists to
practice (or to exercise tooling for):

- detecting legacy testability blockers and choosing a seam or fallback
  strategy
- characterization testing of code with no tests and no documentation
- detecting and refactoring test smells in an existing, badly-tested suite
- deciding, for one codebase, which parts need "add tests" work and which
  parts need "fix the existing tests" work

Unlike `16-smelly_tic_tac_toe` (which bakes in **production code smells** to
refactor), this kata bakes in **testability blockers** and **test smells** —
the two things that make legacy code hard to safely add tests to.

Neither `kata.h`/`kata.c` nor `test_kata.c` contain any explanatory
comments about *where* the problems are. Finding and naming each blocker and
each smell is part of the exercise — this README intentionally does not say
where they are.

## Current State

| Area                                                                      | Coverage | What's wrong                                                 |
| -------------------------------------------------------------------------- | -------- | -------------------------------------------------------------- |
| `Rover`, `ObstacleSensor`, `RadioTransmitter`, `MissionClock` (mission_clock_now) | **0%**   | no tests at all                                               |
| `CommandTranslator`, `MissionLog`, `Coordinate`                            | tested   | tests exist in `test_kata.c`, but nearly all of them are smelly |

## Problem Description

A rover lands on a square plateau (`grid_size x grid_size`, wrapping at the
edges) and receives a string of commands:

- `L`: rotate 90° left
- `R`: rotate 90° right
- `M`: move forward one grid point

A hidden `ObstacleSensor` blocks a move into specific grid squares; when
that happens the rover reports the obstacle over `RadioTransmitter` and
stops where it last was, returning `O x y direction`. Otherwise it returns
its final `x y direction`.

Commands can also arrive in Spanish, French, Portuguese, or Italian and must
be translated to `L`/`R`/`M` first (`CommandTranslator`), and every
translated sequence is written to a `TelemetryPort` (`MissionLog`).

## What to Look For

The 0%-covered part of `kata.c` has several deliberate testability blockers
— things that make a function or struct hard or impossible to put under
test as-is. Before writing a single test, identify each one and name it
using a legacy-code testability taxonomy of your choice (constructor/factory
function does real work, hardcoded dependency, static/global state,
"static"-style function with a side effect, non-determinism with no seam,
etc.).

The tests in `test_kata.c` currently all pass, but the suite is deliberately
smelly. It represents every smell from this catalogue at least once:

Logic in Test, Mock Overuse, Test Interdependence, Fragile Test, Mystery
Guest, Eager Test, Assertion Roulette, Obscure Test, Test Code Duplication,
Conditional Test Logic, Hard-Coded Test Data, Testing Private Methods, Slow
Unit Test, Mocking Final/Concrete Classes, Mocking Value Objects,
Implementation Coupling, Shared Mutable State, Port-Boundary Violations,
Testing Theater.

Some smells overlap in the same test. None of them are labeled in the code.

Note: C has no `private` keyword and no reflection-based "cast past the
access modifier" trick that the original TypeScript smelly test used. To
still demonstrate the "Testing Private Methods" smell, the C port exposes
the translation helpers (`command_translator_translate_spanish` and
friends) directly in `kata.h`, even though they are implementation details
of `command_translator_translate`. That exposure — a header leaking an
internal helper just so a test can reach it — is the idiomatic C shape of
the same smell. See `PORTING_NOTES_C.md` in `ts-kata-answers` for more on
this and other C-specific adaptations.

## Tasks

Pick one direction (or both, on separate branches):

1. **Add tests, outside-in** — `Rover` has none. Start at the acceptance
   level (a full command string in, a result string out), then
   unit-characterize `ObstacleSensor`'s obstacle grid, then decide what seam
   each hardcoded dependency needs before any of that is even possible.
2. **Fix the existing tests** — everything in `test_kata.c` currently
   passes, but almost none of it protects real behavior. Detect each smell,
   then refactor it without ever weakening or deleting a test to make it
   pass.

## Tips

- Do not fix a testability blocker and add new tests in the same change —
  seam refactors are a separate, behavior-preserving change from test
  additions.
- The smelly tests currently pass. That is the point: passing is not the
  same as protecting anything.
- Run mutation testing after any test addition or fix — high coverage on
  this file with weak assertions is exactly what several of the smells
  above look like from the outside.

## Run

```sh
cmake -B build
cmake --build build
./build/test_21-smelly_mars_rover
```
