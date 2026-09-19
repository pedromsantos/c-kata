#include "transport_selector.h"

#include <stdio.h>
#include <string.h>

TransportSelector transport_selector_create(void) {
    TransportSelector selector;
    selector.transport[0] = '\0';
    return selector;
}

/* Connascence of Meaning: "1".."4" are magic strings whose meaning (bike,
 * car, train, bus) is an unstated convention shared with every caller. */
void transport_selector_set_transport(TransportSelector* self, const char* transport_code) {
    if (strcmp(transport_code, "1") == 0) {
        snprintf(self->transport, sizeof(self->transport), "%s", "bike");
    } else if (strcmp(transport_code, "2") == 0) {
        snprintf(self->transport, sizeof(self->transport), "%s", "car");
    } else if (strcmp(transport_code, "3") == 0) {
        snprintf(self->transport, sizeof(self->transport), "%s", "train");
    } else if (strcmp(transport_code, "4") == 0) {
        snprintf(self->transport, sizeof(self->transport), "%s", "bus");
    } else {
        snprintf(self->transport, sizeof(self->transport), "%s", "unknown");
    }
}
