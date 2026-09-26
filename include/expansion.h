#pragma once

#include "lexer.h"

void expand_environment(tokenlist *tokens);
void expand_tilde(tokenlist *tokens);
