#pragma once

#include <sys/types.h>

#define MAX_JOBS 10 /* spec guarantees no more than 10 concurrent bg jobs */

void add_job(pid_t pid, const char *cmdline);

void check_jobs(void);

void print_jobs(void);

void wait_for_all_jobs(void);
