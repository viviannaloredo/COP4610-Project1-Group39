#include "lexer.h"
#include "prompt.h"
#include "expansion.h"
#include "path_search.h"
#include "executor.h"
#include "pipeline.h"
#include "builtins.h"
#include "history.h"
#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
 
/* Removes a trailing "&" token. Returns 1 if the command should run in the background. */
static int take_background_flag(tokenlist *tokens)
{
    if (tokens->size == 0 || strcmp(tokens->items[tokens->size - 1], "&") != 0)
        return 0;
 
    free(tokens->items[tokens->size - 1]);
    tokens->size--;
    tokens->items[tokens->size] = NULL;
    return 1;
}
 
/* Joins tokens with spaces (used to remember the command line for jobs/history) */
static char *join_tokens(tokenlist *tokens)
{
    size_t length = 1;
    for (size_t i = 0; i < tokens->size; i++)
        length += strlen(tokens->items[i]) + 1;
 
    char *line = malloc(length);
    line[0] = '\0';
    for (size_t i = 0; i < tokens->size; i++) {
        if (i > 0)
            strcat(line, " ");
        strcat(line, tokens->items[i]);
    }
    return line;
}
 
/* Runs cd or jobs (exit never returns). Returns 1 if the command was valid. */
static int run_internal(tokenlist *tokens)
{
    if (strcmp(tokens->items[0], "cd") == 0)
        return builtin_cd(tokens) == 0;
 
    run_builtin(tokens);
    return 1;
}
 
/* Runs a single external command. Returns 1 if it was found and started. */
static int run_external(tokenlist *tokens, int background, const char *cmdline)
{
    char *path = find_command_path(tokens->items[0]);
    if (path == NULL) {
        fprintf(stderr, "%s: command not found\n", tokens->items[0]);
        return 0;
    }
 
    pid_t pid = execute_command(tokens, path);
    free(path);
    if (pid < 0)
        return 0;
 
    if (background)
        add_job(pid, cmdline);
    else
        waitpid(pid, NULL, 0);
    return 1;
}
 
/* Runs cmd1 | cmd2 | ... Returns 1 if every command was found and started. */
static int run_pipeline(tokenlist *tokens, int background, const char *cmdline)
{
    size_t num_stages = 0;
    tokenlist **stages = split_pipeline(tokens, &num_stages);
    if (stages == NULL)
        return 0;
 
    char **paths = malloc(num_stages * sizeof(char *));
    for (size_t i = 0; i < num_stages; i++)
        paths[i] = find_command_path(stages[i]->items[0]);
 
    pid_t *pids = execute_pipeline(stages, paths, num_stages);
    int valid = (pids != NULL);
 
    if (pids != NULL) {
        if (background) {
            /* The job is shown with the last command's PID */
            add_job(pids[num_stages - 1], cmdline);
        } else {
            for (size_t i = 0; i < num_stages; i++)
                waitpid(pids[i], NULL, 0);
        }
        free(pids);
    }
 
    for (size_t i = 0; i < num_stages; i++) {
        free(paths[i]);
        free_tokens(stages[i]);
    }
    free(paths);
    free(stages);
    return valid;
}
 
int main(void)
{
    /*
     * Read input unbuffered so this shell never reads ahead past the
     * current line. That lets a nested shell run inside it (shell-ception).
     */
    setvbuf(stdin, NULL, _IONBF, 0);
 
    while (1) {
        print_prompt();
 
        char *input = get_input();
        if (input == NULL) {       /* Ctrl-D works like exit */
            printf("\n");
            builtin_exit();
        }
 
        /* Report background jobs that finished since the last command */
        check_jobs();
        fflush(stdout);
 
        tokenlist *tokens = get_tokens(input);
        free(input);
 
        int background = take_background_flag(tokens);
        if (tokens->size == 0) {
            free_tokens(tokens);
            continue;
        }
 
        /* Save the command as typed (before expansion) for jobs and history */
        char *cmdline = join_tokens(tokens);
 
        expand_environment(tokens);
        expand_tilde(tokens);
 
        int valid;
        if (is_builtin(tokens->items[0]))
            valid = run_internal(tokens);
        else if (has_pipe(tokens))
            valid = run_pipeline(tokens, background, cmdline);
        else
            valid = run_external(tokens, background, cmdline);
 
        if (valid)
            add_history(cmdline);
 
        free(cmdline);
        free_tokens(tokens);
    }
    return 0;
}
 
