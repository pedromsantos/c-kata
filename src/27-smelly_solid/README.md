# SOLID Violations Kata

## Overview

This is a **verification fixture, not a practice exercise**. Each file
contains a small, self-contained example of exactly one SOLID principle
violation, translated directly from *Agile Technical Practices Distilled*'s
SOLID chapter worked examples (Car/`Save`, `CarEngineStatusReportController`,
`Chef`/`Oven`/`Microwave`, `IAmACar`, `Kitchen`/`MicrowaveOven`), adapted to
C's function-pointer-struct idiom for anything the original examples express
via classes/interfaces/inheritance. Its purpose is to give static-analysis/
AI code-review tooling (specifically
[jev-review](https://github.com/pedromsantos/jev-review)) a known-answer set
to check its SOLID rules against -- every file's violation is deliberate and
documented below, not hidden.

Equivalent kata exist for Go, Java, Python, and C# too.

## What's here

| File                                | Violation | What it shows                                                                                          |
| ------------------------------------ | --------- | -------------------------------------------------------------------------------------------------------- |
| `Srp/car.h` / `Srp/car.c`           | SRP       | `Car` mixes domain behaviour (mileage/travel) with a persistence concern (`car_save`).                  |
| `Ocp/car_engine_report.h` / `.c`    | OCP       | `car_engine_status_report_controller_*` must be edited (a new function added) for every new report format; it also calls the concrete `_fill_with` functions directly, a legitimate compound DIP reading kept intentionally. |
| `Lsp/oven.h` / `Lsp/oven.c`         | LSP       | `Microwave`'s `cook_fn` always fails instead of honouring `Oven`'s contract; `chef_cook`'s kind-check is the diagnostic tell, not the violation itself. |
| `Isp/electric_car.h` / `.c`         | ISP       | `IAmACar` bundles a gasoline-refill capability no electric car can honestly support; `ElectricCar`'s `refill_gasoline_fn` is a forced stub. |
| `Dip/kitchen.h` / `Dip/kitchen.c`   | DIP       | `microwave_oven_create()` builds a concrete `MicrowaveGenerator` itself, and `kitchen_create()` builds a concrete `MicrowaveOven` itself -- neither depends on an injected abstraction. |
