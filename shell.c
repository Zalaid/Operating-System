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

