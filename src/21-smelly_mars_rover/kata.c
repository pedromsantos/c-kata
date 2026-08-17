#include "kata.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ---- Coordinate --------------------------------------------------------- */

int coordinate_equals(Coordinate a, Coordinate b) {
    return a.x == b.x && a.y == b.y;
}

/* ---- ObstacleSensor ------------------------------------------------------ */

static const char* obstacle_grid[] = {"1,2", "3,3", "0,4"};

ObstacleSensor obstacle_sensor_create(void) {
    ObstacleSensor sensor;
    sensor.obstacles = obstacle_grid;
    sensor.obstacle_count = 3;
    return sensor;
}

int obstacle_sensor_detects_obstacle_at(ObstacleSensor* sensor, int x, int y) {
    double sensor_noise = (double)rand() / (double)RAND_MAX;
    if (sensor_noise < 0.0001) {
        return 1;
    }

    char position[32];
    snprintf(position, sizeof(position), "%d,%d", x, y);

    for (int i = 0; i < sensor->obstacle_count; i++) {
        if (strcmp(sensor->obstacles[i], position) == 0) {
            return 1;
        }
    }
    return 0;
}

/* ---- RadioTransmitter ----------------------------------------------------- */

RadioTransmitter radio_transmitter_create(const char* endpoint) {
    RadioTransmitter radio;
    radio.endpoint = endpoint;
    return radio;
}

void radio_transmitter_send(RadioTransmitter* radio, const char* message) {
    printf("[RADIO -> %s] %s\n", radio->endpoint, message);
}

/* ---- MissionClock ----------------------------------------------------------- */

void mission_clock_now(char* buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm utc;
#if defined(_WIN32)
    gmtime_s(&utc, &now);
#else
    gmtime_r(&now, &utc);
#endif
    char stamp[24];
    strftime(stamp, sizeof(stamp), "%Y-%m-%dT%H:%M:%S", &utc);
    snprintf(buffer, buffer_size, "%s.000Z", stamp);
}

/* ---- Rover -------------------------------------------------------------------- */

static void rover_turn_left(Rover* rover) {
    static const char order[] = {'N', 'W', 'S', 'E'};
    for (int i = 0; i < 4; i++) {
        if (order[i] == rover->direction) {
            rover->direction = order[(i + 1) % 4];
            return;
        }
    }
}

static void rover_turn_right(Rover* rover) {
    static const char order[] = {'N', 'E', 'S', 'W'};
    for (int i = 0; i < 4; i++) {
        if (order[i] == rover->direction) {
            rover->direction = order[(i + 1) % 4];
            return;
        }
    }
}

static void rover_move_forward(Rover* rover) {
    if (rover->direction == 'N') {
        rover->y = (rover->y + 1) % rover->grid_size;
    } else if (rover->direction == 'S') {
        rover->y = (rover->y - 1 + rover->grid_size) % rover->grid_size;
    } else if (rover->direction == 'E') {
        rover->x = (rover->x + 1) % rover->grid_size;
    } else if (rover->direction == 'W') {
        rover->x = (rover->x - 1 + rover->grid_size) % rover->grid_size;
    }
}

static void rover_report_obstacle(Rover* rover) {
    char timestamp[32];
    mission_clock_now(timestamp, sizeof(timestamp));

    char message[128];
    snprintf(message, sizeof(message), "OBSTACLE %d %d %c at %s", rover->x, rover->y, rover->direction, timestamp);
    radio_transmitter_send(&rover->radio, message);
}

Rover* rover_create(int x, int y, char direction, int grid_size) {
    Rover* rover = malloc(sizeof(Rover));
    if (rover) {
        rover->x = x;
        rover->y = y;
        rover->direction = direction;
        rover->grid_size = grid_size;
        rover->sensor = obstacle_sensor_create();
        rover->radio = radio_transmitter_create("mission-control.nasa.gov");
    }
    return rover;
}

void rover_destroy(Rover* rover) {
    free(rover);
}

char* rover_execute(Rover* rover, const char* commands) {
    for (const char* command = commands; *command != '\0'; command++) {
        Position previous = {rover->x, rover->y};

        if (*command == 'L') rover_turn_left(rover);
        else if (*command == 'R') rover_turn_right(rover);
        else if (*command == 'M') rover_move_forward(rover);

        if (obstacle_sensor_detects_obstacle_at(&rover->sensor, rover->x, rover->y)) {
            rover->x = previous.x;
            rover->y = previous.y;
            rover_report_obstacle(rover);

            char* result = malloc(32);
            snprintf(result, 32, "O %d %d %c", rover->x, rover->y, rover->direction);
            return result;
        }
    }

    char* result = malloc(32);
    snprintf(result, 32, "%d %d %c", rover->x, rover->y, rover->direction);
    return result;
}

/* ---- CommandTranslator ---------------------------------------------------------- */

static char last_language_used[8] = "EN";

char command_translator_translate_spanish(char command) {
    if (command == 'I') return 'L';
    if (command == 'D') return 'R';
    if (command == 'A') return 'M';
    return command;
}

char command_translator_translate_french(char command) {
    if (command == 'G') return 'L';
    if (command == 'D') return 'R';
    if (command == 'A') return 'M';
    return command;
}

char command_translator_translate_portuguese(char command) {
    if (command == 'E') return 'L';
    if (command == 'D') return 'R';
    if (command == 'A') return 'M';
    return command;
}

char command_translator_translate_italian(char command) {
    if (command == 'S') return 'L';
    if (command == 'D') return 'R';
    if (command == 'A') return 'M';
    return command;
}

CommandTranslator command_translator_create(void) {
    CommandTranslator translator;
    translator.translate_sequence_fn = NULL;
    return translator;
}

char command_translator_translate(CommandTranslator* translator, char command, const char* language) {
    (void)translator;
    snprintf(last_language_used, sizeof(last_language_used), "%s", language);

    if (strcmp(language, "EN") == 0) return command;
    if (strcmp(language, "ES") == 0) return command_translator_translate_spanish(command);
    if (strcmp(language, "FR") == 0) return command_translator_translate_french(command);
    if (strcmp(language, "PT") == 0) return command_translator_translate_portuguese(command);
    if (strcmp(language, "IT") == 0) return command_translator_translate_italian(command);

    return command;
}

static char* command_translator_translate_sequence_default(CommandTranslator* translator, const char* commands,
                                                             const char* language) {
    size_t length = strlen(commands);
    char* result = malloc(length + 1);
    for (size_t i = 0; i < length; i++) {
        result[i] = command_translator_translate(translator, commands[i], language);
    }
    result[length] = '\0';
    return result;
}

char* command_translator_translate_sequence(CommandTranslator* translator, const char* commands,
                                             const char* language) {
    if (translator->translate_sequence_fn) {
        return translator->translate_sequence_fn(translator, commands, language);
    }
    return command_translator_translate_sequence_default(translator, commands, language);
}

const char* command_translator_get_last_language_used(void) {
    return last_language_used;
}

/* ---- MissionLog -------------------------------------------------------------------- */

MissionLog mission_log_create(CommandTranslator* translator, TelemetryPort* telemetry) {
    MissionLog log;
    log.translator = translator;
    log.telemetry = telemetry;
    return log;
}

char* mission_log_log_translated_sequence(MissionLog* log, const char* commands, const char* language) {
    char* translated = command_translator_translate_sequence(log->translator, commands, language);

    size_t entry_size = strlen(language) + 1 + strlen(commands) + 2 + strlen(translated) + 1;
    char* entry = malloc(entry_size);
    snprintf(entry, entry_size, "%s:%s->%s", language, commands, translated);

    log->telemetry->record(log->telemetry, entry);

    free(entry);
    return translated;
}
