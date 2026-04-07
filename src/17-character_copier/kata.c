#include "kata.h"
#include <stdlib.h>

Copier* copier_create(void) {
    Copier* copier = malloc(sizeof(Copier));
    if (copier) {
        copier->data = NULL;
    }
    return copier;
}

void copier_destroy(Copier* copier) {
    if (copier) {
        free(copier);
    }
}

void copier_copy(Copier* copier) {
    (void)copier;
    return;
}

Source* source_create(void) {
    Source* source = malloc(sizeof(Source));
    if (source) {
        source->data = NULL;
    }
    return source;
}

void source_destroy(Source* source) {
    if (source) {
        free(source);
    }
}

char source_get_char(Source* source) {
    (void)source;
    return '\0';
}

Destination* destination_create(void) {
    Destination* dest = malloc(sizeof(Destination));
    if (dest) {
        dest->data = NULL;
    }
    return dest;
}

void destination_destroy(Destination* dest) {
    if (dest) {
        free(dest);
    }
}

void destination_set_char(Destination* dest, char c) {
    (void)dest;
    (void)c;
}
