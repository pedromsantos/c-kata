#pragma once

/*
 * SOLID Violations Kata - Open/Closed Principle, C port of
 * Ocp/CarEngineWebView.ts, Ocp/CarEnginePrintView.ts and
 * Ocp/CarEngineStatusReportController.ts.
 *
 * OCP violation: every new report format needs a new fill function (and a
 * new case wired into the controller below) -- the controller must be
 * edited, not extended, to add a case. It also legitimately reads as a DIP
 * violation (the controller calls the concrete fill functions directly);
 * that compound reading is intentional, matching the book's own text -- see
 * README.md.
 */

#include <stddef.h>

typedef struct {
    double rpm;
    double temperature;
} CarEngineViewModel;

void car_engine_web_view_fill_with(char* buffer, size_t buffer_size, CarEngineViewModel view_model);
void car_engine_print_view_fill_with(char* buffer, size_t buffer_size, CarEngineViewModel view_model);

void car_engine_status_report_controller_display(CarEngineViewModel view_model, char* buffer, size_t buffer_size);
void car_engine_status_report_controller_print(CarEngineViewModel view_model, char* buffer, size_t buffer_size);
