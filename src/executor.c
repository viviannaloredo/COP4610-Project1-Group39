#include "executor.h"
#include "redirection.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pid_t execute_command(tokenlist *tokens, const char *exec_path)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        if (setup_redirection(tokens) != 0) {
            _exit(1);
        }

        /* execv expects argv[0] to contain the resolved executable path. */
        tokens->items[0] = (char *)exec_path;
        execv(exec_path, tokens->items);

        /* execv() only returns on failure. */
        fprintf(stderr, "%s: could not execute\n", tokens->items[0]);
        _exit(127);
    }

    return pid;
}
