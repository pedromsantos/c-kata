#pragma once

#define MARS_ROVER "Mars plateau"

typedef struct {
    void* data;
} Rover;

typedef struct {
    void* data;
} Radio;

Rover* rover_create(void);
void rover_destroy(Rover* rover);
void rover_execute(Rover* rover);

Radio* radio_create(void);
void radio_destroy(Radio* radio);
void radio_send(Radio* radio, const char* message);
const char* radio_receive(Radio* radio);
