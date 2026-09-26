#include "path_search.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *find_command_path(const char *command)
{
    if (command == NULL || command[0] == '\0')
        return NULL;

    /* Commands containing a slash already specify their path. */
    if (strchr(command, '/') != NULL) {
        if (access(command, F_OK) == 0) {
            char *result = malloc(strlen(command) + 1);

            if (result == NULL)
                return NULL;

            strcpy(result, command);
            return result;
        }

        return NULL;
    }

    const char *path = getenv("PATH");

    if (path == NULL)
        return NULL;

    /* strtok() changes its input, so work on a copy of PATH. */
    char *path_copy = malloc(strlen(path) + 1);

    if (path_copy == NULL)
        return NULL;

    strcpy(path_copy, path);

    char *directory = strtok(path_copy, ":");

    while (directory != NULL) {
        size_t length = strlen(directory) + strlen(command) + 2;
        char *candidate = malloc(length);

        if (candidate == NULL) {
            free(path_copy);
            return NULL;
        }

        strcpy(candidate, directory);
        strcat(candidate, "/");
        strcat(candidate, command);

        if (access(candidate, F_OK) == 0) {
            free(path_copy);
            return candidate;
        }

        free(candidate);
        directory = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}
