#ifndef CHARACTER_COPIER_H
#define CHARACTER_COPIER_H

typedef struct {
    void* data;
} Copier;

typedef struct {
    void* data;
} Source;

typedef struct {
    void* data;
} Destination;

Copier* copier_create(void);
void copier_destroy(Copier* copier);
void copier_copy(Copier* copier);

Source* source_create(void);
void source_destroy(Source* source);
char source_get_char(Source* source);

Destination* destination_create(void);
void destination_destroy(Destination* destination);
void destination_set_char(Destination* dest, char c);

#endif /* CHARACTER_COPIER_H */
