#define _POSIX_C_SOURCE 200809L

#include "redirection.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static size_t find_symbol(tokenlist *tokens, const char *symbol)
{
    for (size_t i = 0; i < tokens->size; i++) {
        if (strcmp(tokens->items[i], symbol) == 0)
            return i;
    }

    return tokens->size;
}

static int is_redirection_symbol(const char *item)
{
    return strcmp(item, "<") == 0 || strcmp(item, ">") == 0;
}

static void remove_redirection(tokenlist *tokens, size_t index)
{
    free(tokens->items[index]);
    free(tokens->items[index + 1]);

    /* Shift the remaining arguments left, including the final NULL. */
    for (size_t i = index; i + 2 <= tokens->size; i++)
        tokens->items[i] = tokens->items[i + 2];

    tokens->size -= 2;
}

int setup_redirection(tokenlist *tokens)
{
    size_t input_index = find_symbol(tokens, "<");
    size_t output_index = find_symbol(tokens, ">");
    int input_fd = -1;
    int output_fd = -1;

    if (input_index != tokens->size) {
        if (input_index + 1 >= tokens->size ||
            is_redirection_symbol(tokens->items[input_index + 1])) {
            fprintf(stderr, "input redirection: missing file\n");
            return -1;
        }
    }

    if (output_index != tokens->size) {
        if (output_index + 1 >= tokens->size ||
            is_redirection_symbol(tokens->items[output_index + 1])) {
            fprintf(stderr, "output redirection: missing file\n");
            return -1;
        }
    }

    /*
     * Open the input first so a bad input file does not create or
     * overwrite the output file.
     */
    if (input_index != tokens->size) {
        input_fd = open(tokens->items[input_index + 1], O_RDONLY);

        if (input_fd == -1) {
            perror("input redirection");
            return -1;
        }

        struct stat info;

        if (fstat(input_fd, &info) == -1) {
            perror("input redirection");
            close(input_fd);
            return -1;
        }

        if (!S_ISREG(info.st_mode)) {
            fprintf(stderr, "input redirection: not a regular file\n");
            close(input_fd);
            return -1;
        }
    }

    if (output_index != tokens->size) {
        output_fd = open(tokens->items[output_index + 1],
                         O_WRONLY | O_CREAT | O_TRUNC,
                         S_IRUSR | S_IWUSR);

        if (output_fd == -1) {
            perror("output redirection");

            if (input_fd != -1)
                close(input_fd);

            return -1;
        }

        /* Required output permissions: -rw------- */
        if (fchmod(output_fd, S_IRUSR | S_IWUSR) == -1) {
            perror("output redirection");
            close(output_fd);

            if (input_fd != -1)
                close(input_fd);

            return -1;
        }
    }

    if (input_fd != -1) {
        if (dup2(input_fd, STDIN_FILENO) == -1) {
            perror("dup2");
            close(input_fd);

            if (output_fd != -1)
                close(output_fd);

            return -1;
        }

        close(input_fd);
    }

    if (output_fd != -1) {
        if (dup2(output_fd, STDOUT_FILENO) == -1) {
            perror("dup2");
            close(output_fd);
            return -1;
        }

        close(output_fd);
    }

    /*
     * Remove the redirection symbols and filenames so execv() will
     * receive only the actual command and its arguments.
     */
    if (input_index != tokens->size && output_index != tokens->size) {
        if (input_index > output_index) {
            remove_redirection(tokens, input_index);
            remove_redirection(tokens, output_index);
        } else {
            remove_redirection(tokens, output_index);
            remove_redirection(tokens, input_index);
        }
    } else if (input_index != tokens->size) {
        remove_redirection(tokens, input_index);
    } else if (output_index != tokens->size) {
        remove_redirection(tokens, output_index);
    }

    return 0;
}
