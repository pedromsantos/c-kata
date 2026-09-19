#include "car_engine_report.h"

#include <stdio.h>

void car_engine_web_view_fill_with(char* buffer, size_t buffer_size, CarEngineViewModel view_model) {
    snprintf(buffer, buffer_size, "<div>%g rpm, %gC</div>", view_model.rpm, view_model.temperature);
}

void car_engine_print_view_fill_with(char* buffer, size_t buffer_size, CarEngineViewModel view_model) {
    snprintf(buffer, buffer_size, "RPM: %g, Temp: %g", view_model.rpm, view_model.temperature);
}

/* OCP violation: adding a new report format means adding a new fill
 * function AND a new dispatch function here, editing this file instead of
 * extending it -- see the header comment. */
void car_engine_status_report_controller_display(CarEngineViewModel view_model, char* buffer, size_t buffer_size) {
    car_engine_web_view_fill_with(buffer, buffer_size, view_model);
}

void car_engine_status_report_controller_print(CarEngineViewModel view_model, char* buffer, size_t buffer_size) {
    car_engine_print_view_fill_with(buffer, buffer_size, view_model);
}
