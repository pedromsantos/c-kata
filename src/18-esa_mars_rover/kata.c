#include "kata.h"
#include <stdlib.h>

Rover* rover_create(void) {
    Rover* rover = malloc(sizeof(Rover));
    if (rover) {
        rover->data = NULL;
    }
    return rover;
}

void rover_destroy(Rover* rover) {
    if (rover) {
        free(rover);
    }
}

void rover_execute(Rover* rover) {
    (void)rover;
}

Radio* radio_create(void) {
    Radio* radio = malloc(sizeof(Radio));
    if (radio) {
        radio->data = NULL;
    }
    return radio;
}

void radio_destroy(Radio* radio) {
    if (radio) {
        free(radio);
    }
}

void radio_send(Radio* radio, const char* message) {
    (void)radio;
    (void)message;
}

const char* radio_receive(Radio* radio) {
    (void)radio;
    return "";
}
