#pragma once

/*
 * Connascence Kata - Connascence of Timing, C port of
 * BackgroundJobRunner.ts.
 *
 * Timing violation: wait_for_result waits an arbitrary fixed delay instead
 * of synchronizing on real completion -- this mirrors the book's
 * Thread.Sleep/arbitrary-timeout anti-pattern, simplified here to
 * synchronous C (no pthreads/async machinery, matching the rest of this
 * repo).
 */

#define BACKGROUND_JOB_RESULT_MAX 64

typedef struct {
    char result[BACKGROUND_JOB_RESULT_MAX];
} BackgroundJobRunner;

BackgroundJobRunner background_job_runner_create(void);
void background_job_runner_start_job(BackgroundJobRunner* self);
const char* background_job_runner_wait_for_result(BackgroundJobRunner* self);
