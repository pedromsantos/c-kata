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

const char* rover_execute(Rover* rover, const char* command) {
    (void)rover;
    return command;
}
