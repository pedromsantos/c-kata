#ifndef MARS_ROVER_H
#define MARS_ROVER_H

typedef struct {
    void* data;
} Rover;

Rover* rover_create(void);
void rover_destroy(Rover* rover);
const char* rover_execute(Rover* rover, const char* command);

#endif /* MARS_ROVER_H */
