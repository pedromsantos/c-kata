#include "background_job_runner.h"

#include <stdio.h>
#include <unistd.h>

BackgroundJobRunner background_job_runner_create(void) {
    BackgroundJobRunner runner;
    runner.result[0] = '\0';
    return runner;
}

void background_job_runner_start_job(BackgroundJobRunner* self) {
    /* Simulates a job that has already run to completion. */
    snprintf(self->result, sizeof(self->result), "%s", "job-result");
}

/* Connascence of Timing: an arbitrary fixed wait unrelated to any real
 * completion signal, standing in for the book's Thread.Sleep example. */
const char* background_job_runner_wait_for_result(BackgroundJobRunner* self) {
    usleep(1000000); /* 1 second, arbitrary */
    return self->result;
}
