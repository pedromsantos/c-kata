#pragma once

/*
 * Connascence Kata - Connascence of Meaning, C port of
 * TransportSelector.ts.
 *
 * Meaning violation: setTransport maps magic strings ("1".."4") to
 * bike/car/train/bus via an unstated convention -- caller and callee must
 * independently agree on what each code means.
 */

#define TRANSPORT_NAME_MAX 16

typedef struct {
    char transport[TRANSPORT_NAME_MAX];
} TransportSelector;

TransportSelector transport_selector_create(void);
void transport_selector_set_transport(TransportSelector* self, const char* transport_code);
