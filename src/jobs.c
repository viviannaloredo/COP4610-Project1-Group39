#include "jobs.h"
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

typedef struct {
    int job_num;
    pid_t pid;
    char cmdline[256];
    int active;
} Job;

static Job job_list[MAX_JOBS];
static int next_job_num = 1;
static int job_count = 0; 

void add_job(pid_t pid, const char *cmdline)
{
    if (job_count >= MAX_JOBS) {
        
        fprintf(stderr, "shell: too many background jobs\n");
        return;
    }

    Job *j = &job_list[job_count++];
    j->job_num = next_job_num++;
    j->pid = pid;
    strncpy(j->cmdline, cmdline, sizeof(j->cmdline) - 1);
    j->cmdline[sizeof(j->cmdline) - 1] = '\0';
    j->active = 1;

    printf("[%d] %d\n", j->job_num, j->pid);
}

void check_jobs(void)
{
    for (int i = 0; i < job_count; i++) {
        if (!job_list[i].active) continue;

        int status;
        pid_t result = waitpid(job_list[i].pid, &status, WNOHANG);

        if (result == job_list[i].pid) {
            printf("[%d]+ done %s\n", job_list[i].job_num, job_list[i].cmdline);
            job_list[i].active = 0;
        }
    }
}

void print_jobs(void)
{
    int any_active = 0;
    for (int i = 0; i < job_count; i++) {
        if (job_list[i].active) {
            printf("[%d]+ %d %s\n", job_list[i].job_num,
                   job_list[i].pid, job_list[i].cmdline);
            any_active = 1;
        }
    }
    if (!any_active) {
        printf("No active background jobs.\n");
    }
}

void wait_for_all_jobs(void)
{
    for (int i = 0; i < job_count; i++) {
        if (job_list[i].active) {
            int status;
            waitpid(job_list[i].pid, &status, 0);
            printf("[%d]+ done %s\n", job_list[i].job_num, job_list[i].cmdline);
            job_list[i].active = 0;
        }
    }
}
