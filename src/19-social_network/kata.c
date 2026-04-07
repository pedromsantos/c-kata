#include "kata.h"
#include <stdlib.h>

UseCase* use_case_create(void) {
    UseCase* use_case = malloc(sizeof(UseCase));
    if (use_case) {
        use_case->data = NULL;
    }
    return use_case;
}

void use_case_destroy(UseCase* use_case) {
    if (use_case) {
        free(use_case);
    }
}

void use_case_execute(UseCase* use_case, const char* command) {
    (void)use_case;
    (void)command;
}

const char* use_case_query(UseCase* use_case, const char* query) {
    (void)use_case;
    (void)query;
    return "";
}
