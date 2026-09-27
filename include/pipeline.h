#pragma once

#include "lexer.h"
#include <sys/types.h>


int has_pipe(tokenlist *tokens);
tokenlist **split_pipeline(tokenlist *tokens, size_t *num_stages);

pid_t *execute_pipeline(tokenlist **stages, char **exec_paths, size_t num_stages);
