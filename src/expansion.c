#include "expansion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void expand_environment(tokenlist *tokens)
{
    for (size_t i = 0; i < tokens->size; i++) {
        if (tokens->items[i][0] == '$') {
            const char *value = getenv(tokens->items[i] + 1);

            if (value == NULL)
                value = "";

            char *expanded = realloc(tokens->items[i], strlen(value) + 1);

            if (expanded == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }

            strcpy(expanded, value);
            tokens->items[i] = expanded;
        }
    }
}

void expand_tilde(tokenlist *tokens)
{
    const char *home = getenv("HOME");

    if (home == NULL)
        home = "";

    for (size_t i = 0; i < tokens->size; i++) {
        char *item = tokens->items[i];

        if (strcmp(item, "~") == 0 ||
            (item[0] == '~' && item[1] == '/')) {

            const char *suffix = item + 1;
            size_t new_size = strlen(home) + strlen(suffix) + 1;

            char *expanded = malloc(new_size);

            if (expanded == NULL) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }

            strcpy(expanded, home);
            strcat(expanded, suffix);

            free(tokens->items[i]);
            tokens->items[i] = expanded;
        }
    }
}
