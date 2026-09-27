#include "pipeline.h"
#include "redirection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int has_pipe(tokenlist *tokens)
{
    for (size_t i = 0; i < tokens->size; i++) {
        if (strcmp(tokens->items[i], "|") == 0)
            return 1;
    }
    return 0;
}

tokenlist **split_pipeline(tokenlist *tokens, size_t *num_stages)
{
    size_t pipe_count = 0;
    for (size_t i = 0; i < tokens->size; i++) {
        if (strcmp(tokens->items[i], "|") == 0)
            pipe_count++;
    }

    size_t n = pipe_count + 1;
    tokenlist **stages = malloc(n * sizeof(tokenlist *));
    if (stages == NULL) return NULL;

    for (size_t i = 0; i < n; i++) {
        stages[i] = new_tokenlist();
    }

    size_t stage_idx = 0;
    int prev_was_pipe = 1; 

    for (size_t i = 0; i < tokens->size; i++) {
        if (strcmp(tokens->items[i], "|") == 0) {
            if (prev_was_pipe) {
                fprintf(stderr, "shell: syntax error near unexpected token `|'\n");
                for (size_t j = 0; j < n; j++) free_tokens(stages[j]);
                free(stages);
                return NULL;
            }
            stage_idx++;
            prev_was_pipe = 1;
        } else {
            add_token(stages[stage_idx], tokens->items[i]);
            prev_was_pipe = 0;
        }
    }

    if (prev_was_pipe) {
        fprintf(stderr, "shell: syntax error near unexpected token `|'\n");
        for (size_t j = 0; j < n; j++) free_tokens(stages[j]);
        free(stages);
        return NULL;
    }

    *num_stages = n;
    return stages;
}

pid_t *execute_pipeline(tokenlist **stages, char **exec_paths, size_t num_stages)
{
    for (size_t i = 0; i < num_stages; i++) {
        if (exec_paths[i] == NULL) {
            fprintf(stderr, "%s: command not found\n", stages[i]->items[0]);
            return NULL;
        }
    }

    pid_t *pids = malloc(num_stages * sizeof(pid_t));
    if (pids == NULL) return NULL;

    int prev_read_fd = -1;

    for (size_t i = 0; i < num_stages; i++) {
        int pipe_fds[2] = {-1, -1};
        int has_next = (i + 1 < num_stages);

        if (has_next && pipe(pipe_fds) == -1) {
            perror("pipe");
            free(pids);
            return NULL;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            free(pids);
            return NULL;
        }

        if (pid == 0) {
            if (prev_read_fd != -1) {
                dup2(prev_read_fd, STDIN_FILENO);
                close(prev_read_fd);
            }
            if (has_next) {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[0]);
                close(pipe_fds[1]);
            }
            if (setup_redirection(stages[i]) != 0) {
                _exit(1);
            }

            execv(exec_paths[i], stages[i]->items);
            fprintf(stderr, "%s: could not execute\n", stages[i]->items[0]);
            _exit(127);
        }

        pids[i] = pid;

        if (prev_read_fd != -1) close(prev_read_fd);
        if (has_next) {
            close(pipe_fds[1]);
            prev_read_fd = pipe_fds[0];
        }
    }

    return pids;
}
