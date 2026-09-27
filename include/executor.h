#pragma once

#include "lexer.h"
#include <sys/types.h>

pid_t execute_command(tokenlist *tokens, const char *exec_path);
