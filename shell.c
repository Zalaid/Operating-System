#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 100

void shell_prompt() {
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    printf("PUCITshell@%s:- ", cwd);
}

void execute_command(char **args) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Failed to fork");
    } else if (pid == 0) {  // Child process
        execvp(args[0], args);
        perror("Command execution failed");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        wait(NULL);  // Wait for child to complete
    }
}

int main() {
    char input[MAX_CMD_LEN];
    char *args[MAX_ARGS];

    while (1) {
        shell_prompt();
        if (fgets(input, MAX_CMD_LEN, stdin) == NULL) {
            printf("\nExiting PUCITshell\n"); //printing
            break;
        }

        input[strcspn(input, "\n")] = 0;  // Remove newline
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            args[++i] = strtok(NULL, " ");
        }


        if (args[0] == NULL) continue;  // Empty command

        execute_command(args);
    }

    return 0;
}