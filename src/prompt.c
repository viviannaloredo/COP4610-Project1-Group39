#include "prompt.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_prompt(void)
{
    const char *user = getenv("USER");
    const char *machine = getenv("MACHINE");
    char cwd[4096];

    if (user == NULL)
        user = "";

    if (machine == NULL)
        machine = "";

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        cwd[0] = '\0';

    printf("%s@%s:%s>", user, machine, cwd);
    fflush(stdout);
}
