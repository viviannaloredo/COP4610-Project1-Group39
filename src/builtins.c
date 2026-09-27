#include "builtins.h"
#include "history.h"
#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int builtin_cd(tokenlist *tokens)
{
    const char *target;

    if (tokens->size > 2) {
        fprintf(stderr, "cd: too many arguments\n");
        return -1;
    }

    if (tokens->size == 1) {
        target = getenv("HOME");

        if (target == NULL) {
            fprintf(stderr, "cd: HOME is not set\n");
            return -1;
        }
    } else {
        target = tokens->items[1];
    }

    if (chdir(target) != 0) {
        perror("cd");
        return -1;
    }

    return 0;
}

void builtin_exit(void)
{
    wait_for_all_jobs();
    print_history();
    free_history();

    exit(0);
}

void builtin_jobs(void)
{
    print_jobs();
}

int is_builtin(const char *command)
{
    return command != NULL &&
           (strcmp(command, "cd") == 0 ||
            strcmp(command, "exit") == 0 ||
            strcmp(command, "jobs") == 0);
}

void run_builtin(tokenlist *tokens)
{
    const char *cmd = tokens->items[0];

    if (strcmp(cmd, "cd") == 0) {
        builtin_cd(tokens);
    } else if (strcmp(cmd, "exit") == 0) {
        builtin_exit();
    } else if (strcmp(cmd, "jobs") == 0) {
        builtin_jobs();
    }
}
