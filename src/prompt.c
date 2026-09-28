#define _DEFAULT_SOURCE 
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
 
    /* $MACHINE is not always set, so fall back to the computer's hostname */
    char host[256];
    if (machine == NULL) {
        if (gethostname(host, sizeof(host)) == 0)
            machine = host;
        else
            machine = "";
    }
 
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        cwd[0] = '\0';
 
    printf("%s@%s:%s>", user, machine, cwd);
    fflush(stdout);
}
