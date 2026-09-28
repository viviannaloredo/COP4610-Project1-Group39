#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 3

static char *history[HISTORY_SIZE];
static size_t history_count = 0;

void add_history(const char *command)
{
    if (command == NULL || command[0] == '\0')
        return;

    char *copy = malloc(strlen(command) + 1);

    if (copy == NULL)
        return;

    strcpy(copy, command);

    if (history_count < HISTORY_SIZE) {
        history[history_count] = copy;
        history_count++;
        return;
    }

    free(history[0]);

    for (size_t i = 1; i < HISTORY_SIZE; i++)
        history[i - 1] = history[i];

    history[HISTORY_SIZE - 1] = copy;
}

void print_history(void)
{
    if (history_count == 0) {
        printf("No valid commands.\n");
        return;
    }

    printf("Last (%zu) valid commands:\n", history_count);

    for (size_t i = 0; i < history_count; i++)
        printf("[%zu]: %s\n", i + 1, history[i]);
}

void free_history(void)
{
    for (size_t i = 0; i < history_count; i++) {
        free(history[i]);
        history[i] = NULL;
    }

    history_count = 0;
}
