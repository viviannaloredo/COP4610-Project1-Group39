# Shell

## Description

This project implements a Unix-style command shell in C for COP4610 Operating Systems. The shell reads commands entered by the user and supports features such as environment variable and tilde expansion, PATH searching, external command execution, I/O redirection, pipelines, background processes, internal commands, job tracking, and command history.

The project was divided between the three group members so that each person worked on multiple parts of the shell. Several related parts were assigned to two people so that they could be developed and checked together. After the separate portions were completed, the project was integrated and tested as a complete shell.

## Group Information

- **Group Number:** 39
- **Course:** COP4610 - Operating Systems
- **Project:** Project 1

## Group Members

- Damian Abrego
- Vivianna Loredo
- Xavier Rosario

---

# Division of Labor

## Part 1: Prompt

**Responsibilities:**
Implement the shell prompt and display the current user, machine, and working directory in the required format.

**Assigned to:**
Vivianna Loredo and Xavier Rosario

## Part 2: Environment Variables

**Responsibilities:**
Handle environment variable expansion for tokens beginning with `$` so commands can use values stored in the user's environment.

**Assigned to:**
Vivianna Loredo and Damian Abrego

## Part 3: Tilde Expansion

**Responsibilities:**
Implement tilde expansion so `~` and paths beginning with `~/` are expanded using the user's home directory.

**Assigned to:**
Vivianna Loredo and Xavier Rosario

## Part 4: PATH Search

**Responsibilities:**
Search the directories stored in the `PATH` environment variable to find executable commands. Commands that already contain a path are handled directly.

**Assigned to:**
Vivianna Loredo and Damian Abrego

## Part 5: External Command Execution

**Responsibilities:**
Create child processes and execute external commands. Foreground commands are waited on by the shell while background commands are allowed to continue running.

**Assigned to:**
Xavier Rosario and Damian Abrego

## Part 6: I/O Redirection

**Responsibilities:**
Implement input and output redirection using `<` and `>`, including opening the correct files, redirecting standard input or output, and removing redirection tokens before executing a command.

**Assigned to:**
Vivianna Loredo and Xavier Rosario

## Part 7: Piping

**Responsibilities:**
Implement command pipelines using `|`. Commands in the pipeline are separated into stages and connected using pipes so the output of one command becomes the input of the next.

**Assigned to:**
Xavier Rosario and Damian Abrego

## Part 8: Background Processing

**Responsibilities:**
Support commands ending in `&`, keep track of background jobs, assign increasing job numbers, report completed jobs, and implement the `jobs` command.

**Assigned to:**
Xavier Rosario and Damian Abrego

## Part 9: Internal Command Execution

**Responsibilities:**
Implement shell commands that must be handled internally, including `cd`, `jobs`, and `exit`.

**Assigned to:**
Vivianna Loredo and Damian Abrego

## Extra Credit

**Responsibilities:**
Work on the optional extended shell functionality, including longer pipelines, combining pipelines with I/O redirection, and running the shell from inside another copy of the shell.

**Assigned to:**
Xavier Rosario and Damian Abrego

---

# Features

The completed shell supports:

- Interactive shell prompt
- External command execution
- Command arguments
- PATH searching
- Direct executable paths
- Environment variable expansion
- Tilde expansion
- `cd`
- Input redirection using `<`
- Output redirection using `>`
- Multi-stage pipelines using `|`
- Background commands using `&`
- Background pipelines
- Background job tracking
- Increasing job numbers
- `jobs`
- Background job completion messages
- Waiting for active background jobs before exiting
- Internal commands
- Last-three-valid-command history
- Invalid command handling
- Memory cleanup when the shell exits

---

# File Listing

```text
COP4610-Project1-Group39/
├── include/
│   ├── builtins.h
│   ├── executor.h
│   ├── expansion.h
│   ├── history.h
│   ├── jobs.h
│   ├── lexer.h
│   ├── path_search.h
│   ├── pipeline.h
│   ├── prompt.h
│   └── redirection.h
│
├── src/
│   ├── builtins.c
│   ├── executor.c
│   ├── expansion.c
│   ├── history.c
│   ├── jobs.c
│   ├── lexer.c
│   ├── main.c
│   ├── path_search.c
│   ├── pipeline.c
│   ├── prompt.c
│   └── redirection.c
│
├── .gitignore
├── Makefile
└── README.md
```

## File Descriptions

- `src/main.c` - Contains the main shell loop and connects the different shell components.
- `src/lexer.c` - Reads input and separates it into tokens.
- `src/prompt.c` - Displays the shell prompt.
- `src/expansion.c` - Handles environment variable and tilde expansion.
- `src/path_search.c` - Searches PATH for external commands.
- `src/executor.c` - Handles execution of individual external commands.
- `src/redirection.c` - Handles input and output redirection.
- `src/pipeline.c` - Handles pipelines containing multiple commands.
- `src/jobs.c` - Tracks and reports background jobs.
- `src/history.c` - Stores and prints the most recent valid commands.
- `src/builtins.c` - Handles internal shell commands.

---

# How to Compile and Execute

## Requirements

The project is written in C and was developed and tested on the linprog Linux environment.

The provided Makefile is used to compile the project.

## Compilation

From the project directory, run:

```bash
make
```

This creates the shell executable at:

```text
bin/shell
```

## Execution

Run the shell with:

```bash
./bin/shell
```

or:

```bash
make run
```

## Cleaning Build Files

To remove the generated object files and executable, run:

```bash
make clean
```

The files inside `obj/` and the generated `bin/shell` executable are build files and are not tracked in the Git repository.

---

## Development Log

### Vivianna Loredo

| Date | Work Completed / Notes |
|---|---|
| 2026-09-09 | Met virtually with the group to go through the project requirements and divide the work. My assigned sections included the prompt, environment variables, tilde expansion, PATH searching, I/O redirection, and internal commands. |
| 2026-09-12 | Started working through the prompt and expansion features. Tested the prompt formatting and worked on replacing environment-variable and tilde tokens with their correct values. |
| 2026-09-18 | Focused on PATH searching and I/O redirection and checked how they behaved with the other completed shell features. During our group update, we also discussed what still needed to be connected and tested. |
| 2026-09-24 | Reviewed the internal commands, especially `cd`, `jobs`, and `exit`, and checked their behavior alongside history and background processing. |
| 2026-09-28 | Helped with the final integration and regression testing of the complete shell. Ran command, pipeline, redirection, background-job, history, extra-credit, and Valgrind tests before the final repository review. |

### Damian Abrego

| Date | Work Completed / Notes |
|---|---|
| 2026-09-09 | Joined the initial virtual planning meeting and took on work involving environment variables, PATH search, external command execution, piping, background processing, internal commands, and extra credit. |
| 2026-09-14 | Worked mainly on external command execution and PATH-related behavior. Checked that commands could be located and started correctly with their arguments. |
| 2026-09-19 | Continued with process-management portions of the project, including pipelines and background execution. Reviewed how child processes and command PIDs needed to be tracked. |
| 2026-09-23 | Worked on the `jobs`/background-process behavior and checked that completed processes were reported correctly while job numbers continued increasing. |
| 2026-09-25 | Reviewed the process-related portions during the final group status meeting and helped identify the remaining integration and extra-credit checks that needed to be completed. |

### Xavier Rosario

| Date | Work Completed / Notes |
|---|---|
| 2026-09-09 | Took part in the project planning meeting and was assigned work on the prompt, tilde expansion, external command execution, I/O redirection, piping, background processing, and extra-credit features. |
| 2026-09-13 | Worked on the prompt and tilde-expansion portions and checked that the shell displayed the expected information while expanding `~` paths correctly. |
| 2026-09-17 | Shifted to command execution and redirection. Tested external commands with arguments and reviewed how input/output files were connected to child processes. |
| 2026-09-21 | Worked through piping and background-command behavior, including combinations involving multiple commands and process tracking. |
| 2026-09-25 | During the final virtual project check, reviewed the remaining pipeline/background functionality and the extra-credit features before the full-shell integration tests were completed. |

---

# Meetings

| Date | Attendees | Format | Discussion / Outcome |
|---|---|---|---|
| 2026-09-09 | Damian Abrego, Vivianna Loredo, Xavier Rosario | Virtual | Reviewed the Project 1 requirements and went through the different shell features that needed to be implemented. Divided the project responsibilities between the three members and discussed which sections depended on one another. Decided to divide closely related features between two people when useful so that the sections could be checked together during integration. |
| 2026-09-18 | Damian Abrego, Vivianna Loredo, Xavier Rosario | Virtual | Each member gave an update on their assigned portions of the project. Discussed completed and unfinished features, how the separate source files would interact, and issues involving PATH search, execution, redirection, piping, and background processing. Identified the remaining work needed before final integration. |
| 2026-09-25 | Damian Abrego, Vivianna Loredo, Xavier Rosario | Virtual | Met for a final check of where the project was at before completing integration. Reviewed which features were finished, discussed remaining testing and cleanup, and checked what still needed to be done with the README and repository. Agreed to finish the final build, functionality tests, memory checks, and documentation before considering the project complete. |

---

# Testing

The completed shell was tested using both individual commands and combinations of shell features.

Testing included:

- Running external commands
- Commands with arguments
- Invalid commands
- Environment variable expansion using `$USER`
- Environment variable expansion using `$HOME`
- Expansion of nonexistent environment variables
- Tilde expansion
- PATH searching
- Direct executable paths
- `cd` with a directory
- `cd` with no argument
- Invalid `cd` paths
- `cd` with too many arguments
- Input redirection
- Output redirection
- Output file permissions
- Input and output redirection together
- Two-stage pipelines
- Three-stage pipelines
- Longer pipelines
- Background commands
- Background pipelines
- `jobs`
- Finished background job cleanup
- Increasing job numbers
- Reuse of completed background job slots
- Maximum number of active background jobs
- Waiting for active jobs before exiting
- Valid-command history
- Excluding invalid commands from history
- Nested shell execution
- Memory cleanup

Example pipeline tests included:

```bash
echo hello | tr a-z A-Z
```

and:

```bash
printf hello | tr a-z A-Z | wc -c
```

The project was also checked using:

```bash
git diff --check
```

with no whitespace errors reported.

---

# Memory Testing

The integrated shell was tested using Valgrind.

The final regression test reported:

```text
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks

All heap blocks were freed -- no leaks are possible

ERROR SUMMARY: 0 errors from 0 contexts
```

This confirmed that the final tested version exited without reported memory leaks or memory errors.

---

# Extra Credit

All three extra-credit features were tested successfully.

## Multiple / Longer Pipelines

The shell successfully handled a longer pipeline containing several commands:

```bash
printf hello | cat | cat | cat | cat | cat | wc -c
```

Output:

```text
5
```

## Piping With I/O Redirection

Piping and output redirection were successfully used in the same command:

```bash
printf hello | tr a-z A-Z > /tmp/extra_credit.txt
```

The file was then checked with:

```bash
cat /tmp/extra_credit.txt
```

Output:

```text
HELLO
```

## Shell-ception

The shell was also tested by starting another copy of the shell from inside the running custom shell:

```bash
./bin/shell
```

A command was successfully executed inside the nested shell:

```bash
echo nested_shell_works
```

Output:

```text
nested_shell_works
```

Exiting the nested shell returned to the original custom shell, and exiting the original shell returned to the normal linprog terminal.

---

# Bugs / Known Issues

No known bugs were identified during the final round of testing.

---

# Considerations

The shell was developed and tested on the linprog environment provided for the course.

The project is separated into multiple source and header files so that the different parts of the shell can be developed, tested, and maintained separately.

Before the final version was completed, the combined project was rebuilt and tested again to make sure the integrated features still worked correctly. The final tested version also completed its Valgrind run with no reported memory leaks or errors.
