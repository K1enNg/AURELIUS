// Building a custom Unix-style interactive command-line shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64
#define DELIMITERS " \t\r\n\a"


char* read_line(void);
char** parse_args(char *line);
int execute_command(char **args);

int main(int agrs, char **argv) {
    char *line;
    char **args;
    int status = 1;

    printf("=== Welcome to Mini-C-Shell ===\n");
    printf("Type 'exit' or press Ctrl+D to quit.\n\n");

    while (status) {
        printf("mini_shell> ");
        fflush(stdout);

        line = read_line();
        args = parse_args(line);
        status = execute_command(args);

        free(line);
        free(args);
    }
    return 0;
}

// read command from standard input
char* read_line(void) {
    char *line = NULL;
    size_t bufsize = 0;

    if (getline(&line, &bufline, stdin) == -1) {
        if(feof(stdin)) {
            printf("[shell existed]\n");
            exit(0);
        } else {
            perror("error reading input.\n");
            exit(1);
        }
    }
    return line;
}

// tokenize input string into array of argument pointers
char** parse_args(char *line) {
    int bufsize = MAX_ARGS;
    int pos = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "allocation error");
        exit(1);
    }

    token = strtok(line, DELIMITERS);
    while (token) {
        tokens[pos++] = token;

        if (pos >= bufsize) {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(strderr, "reallocation error.\n");
                exit(1);
            }
        }
        token = strtok(NULL, DELIMITERS);
    }
    tokens[pos] = NULL;
    return tokens;
}

int launch_process(char **args) {
    pid_t pid;
    pid_t wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("mini_shell"); //executed only if command fails
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("error forking process");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

// handle built-in shell commands
int execute_command(char **args) {
    if(args[0] == NULL) {
        return 1;
    }

    if (strcmp(args[0], "exit") == 0) {
        return 0;
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "mini_shell: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("mini_shell");
            }
        }
        return 1;
    }
    return launch_process(args);
}