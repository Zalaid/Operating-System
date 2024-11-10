// myshellv3.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT 1024
#define PROMPT "PUCITshell:- "

void execute_command(char *input) {
    char *args[MAX_INPUT / 2 + 1];
    int background = 0;
    int i = 0;

    if (input[strlen(input) - 1] == '&') {
        background = 1;
        input[strlen(input) - 1] = '\0';
    }

    char *token = strtok(input, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("Execution failed");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("Fork failed");
    } else {
        if (!background) waitpid(pid, NULL, 0);
    }
}

void handle_signal(int sig) {
    waitpid(-1, NULL, WNOHANG);
}

int main() {
    signal(SIGCHLD, handle_signal);
    char input[MAX_INPUT];

    while (1) {
        printf("%s", PROMPT);
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\nExiting shell...\n");
            break;
        }
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0) {
            execute_command(input);
        }
    }

    return 0;
}

