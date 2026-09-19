# Connascence Violations Kata

## Overview

This is a **verification fixture, not a practice exercise**. Each file
contains a small, self-contained example of exactly one form of
connascence, translated directly from *Agile Technical Practices
Distilled*'s Connascence chapter worked examples (`NotificationSystem`,
`TransportSelector`, `ChecksumCalculator`, `ReceiptSender`,
`BackgroundJobRunner`, and the module-level `Counter` singleton), adapted to
C's idioms for anything the original examples express via classes or
module-level exports. Its purpose is to give static-analysis/AI code-review
tooling (specifically [jev-review](https://github.com/pedromsantos/jev-review))
a known-answer set to check its connascence rules against -- every file's
violation is deliberate and documented below, not hidden.

Connascence of Value (CoV) and Connascence of Meaning across Time (CoMT) are
intentionally not represented here: CoV is already covered by jev-review's
existing checks, and CoMT is connascence with an external, undocumented
manual step that no code review can see.

Equivalent kata exist for Go, Java, Python, and C# too.

## What's here

| File                                                | Violation                | What it shows                                                                                       |
| ----------------------------------------------------- | -------------------------- | -------------------------------------------------------------------------------------------------------- |
| `notification_system.h` / `.c`                      | Position                 | `notification_system_send_email` takes three same-typed strings whose meaning is conveyed only by argument position. |
| `transport_selector.h` / `.c`                       | Meaning                  | `transport_selector_set_transport` maps magic strings (`"1"`-`"4"`) to transport names via an unstated convention. |
| `checksum_calculator.h` / `.c`                      | Algorithm                | `checksum_calculator_add_checksum` and `checksum_calculator_check` each independently reimplement the same `sum % 10` loop instead of sharing one helper. |
| `receipt_sender.h` / `.c`                           | Execution Order          | `receipt_sender_archive` is only correct after `receipt_sender_send_to_customer` has run, and nothing enforces that order. |
| `background_job_runner.h` / `.c`                    | Timing                   | `background_job_runner_wait_for_result` waits an arbitrary fixed delay instead of synchronizing on real completion. |
| `identity.h` / `.c` + `counter_consumer.h` / `.c`   | Identity                 | `global_counter` is a single, file-scope shared instance; every caller of its accessors depends on there being exactly one shared instance. |
