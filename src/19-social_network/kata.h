#pragma once

typedef struct {
    void* data;
} UseCase;

UseCase* use_case_create(void);
void use_case_destroy(UseCase* use_case);
void use_case_execute(UseCase* use_case, const char* command);
const char* use_case_query(UseCase* use_case, const char* query);
