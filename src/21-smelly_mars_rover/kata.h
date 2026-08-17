#pragma once

/*
 * Legacy Code & Test Smells Kata - Mars Rover (C port).
 *
 * This header intentionally contains no explanatory comments about what is
 * wrong with it. Finding and naming each testability blocker (in the
 * untested part) and each test smell (in test_kata.c) is part of the
 * exercise. See README.md.
 */

#include <stddef.h>

/* ---- Position / Coordinate ------------------------------------------- */

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int x;
    int y;
} Coordinate;

int coordinate_equals(Coordinate a, Coordinate b);

/* ---- ObstacleSensor ---------------------------------------------------- */

typedef struct {
    const char** obstacles;
    int obstacle_count;
} ObstacleSensor;

ObstacleSensor obstacle_sensor_create(void);
int obstacle_sensor_detects_obstacle_at(ObstacleSensor* sensor, int x, int y);

/* ---- RadioTransmitter --------------------------------------------------- */

typedef struct {
    const char* endpoint;
} RadioTransmitter;

RadioTransmitter radio_transmitter_create(const char* endpoint);
void radio_transmitter_send(RadioTransmitter* radio, const char* message);

/* ---- MissionClock -------------------------------------------------------- */

/* Fills buffer (must be at least 21 bytes) with an ISO-8601 UTC timestamp. */
void mission_clock_now(char* buffer, size_t buffer_size);

/* ---- Rover ---------------------------------------------------------------- */

typedef struct {
    int x;
    int y;
    char direction;
    int grid_size;
    ObstacleSensor sensor;
    RadioTransmitter radio;
} Rover;

Rover* rover_create(int x, int y, char direction, int grid_size);
void rover_destroy(Rover* rover);

/* Caller owns the returned string and must free() it. */
char* rover_execute(Rover* rover, const char* commands);

/* ---- TelemetryPort (hexagonal-ish seam used by MissionLog) --------------- */

typedef struct TelemetryPort {
    void (*record)(struct TelemetryPort* self, const char* entry);
} TelemetryPort;

/* ---- CommandTranslator ----------------------------------------------------
 *
 * Note: translate_spanish/french/portuguese/italian are exposed here (not
 * kept file-static in kata.c) purely so a test can reach into them
 * directly -- that exposure is itself the C analogue of the TypeScript
 * "cast past `private`" smell (see PORTING_NOTES_C.md in ts-kata-answers).
 */

typedef struct CommandTranslator {
    /* Swappable so a test can fake a "concrete class" instance, mirroring
     * the TS smell of casting a plain object to CommandTranslator. Left
     * NULL to use the real implementation. */
    char* (*translate_sequence_fn)(struct CommandTranslator* self, const char* commands, const char* language);
} CommandTranslator;

CommandTranslator command_translator_create(void);

char command_translator_translate(CommandTranslator* translator, char command, const char* language);

/* Caller owns the returned string and must free() it. */
char* command_translator_translate_sequence(CommandTranslator* translator, const char* commands, const char* language);

const char* command_translator_get_last_language_used(void);

char command_translator_translate_spanish(char command);
char command_translator_translate_french(char command);
char command_translator_translate_portuguese(char command);
char command_translator_translate_italian(char command);

/* ---- MissionLog ------------------------------------------------------------ */

typedef struct {
    CommandTranslator* translator;
    TelemetryPort* telemetry;
} MissionLog;

MissionLog mission_log_create(CommandTranslator* translator, TelemetryPort* telemetry);

/* Caller owns the returned string and must free() it. */
char* mission_log_log_translated_sequence(MissionLog* log, const char* commands, const char* language);
