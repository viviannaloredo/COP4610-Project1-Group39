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
