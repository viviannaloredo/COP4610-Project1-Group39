#pragma once

#include "lexer.h"

int builtin_cd(tokenlist *tokens);

void builtin_exit(void);
 
void builtin_jobs(void);
 
int is_builtin(const char *command);
 
void run_builtin(tokenlist *tokens);
