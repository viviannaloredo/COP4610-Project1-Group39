#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
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
