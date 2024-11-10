// myshellv1.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define PROMPT "PUCITshell:- "

void execute_command(char *input) {
    char *args[MAX_INPUT / 2 + 1];
    int i = 0;
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
        wait(NULL);
    }
}

int main() {
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

