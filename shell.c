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


#include <fcntl.h>

void execute_redirection(char **args) {
    int in = -1, out = -1;
    char *cmd[MAX_ARGS];
    int i, j = 0;

    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            in = open(args[++i], O_RDONLY);
        } else if (strcmp(args[i], ">") == 0) {
            out = open(args[++i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        } else {
            cmd[j++] = args[i];
        }
    }
    cmd[j] = NULL;

   
// Update main loop to check for redirection


void execute_command_bg(char **args) {
    pid_t pid = fork();
    if (pid == 0) {  // Child process
        execvp(args[0], args);
        perror("Command execution failed");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        printf("[PID %d] %s started in background\n", pid, args[0]);
    }
}

int main() {
    // Use execute_command_bg when detecting '&' at the end of args
}

#define HISTORY_SIZE 10
char *history[HISTORY_SIZE];
int history_count = 0;

void add_to_history(const char *cmd) {
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(cmd);
    } else {
        free(history[0]);
        memmove(history, history + 1, sizeof(char *) * (HISTORY_SIZE - 1));
        history[HISTORY_SIZE - 1] = strdup(cmd);
    }
}

#define HISTORY_SIZE 10
char *history[HISTORY_SIZE];
int history_count = 0;

void add_to_history(const char *cmd) {
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(cmd);
    } else {
        free(history[0]);
        memmove(history, history + 1, sizeof(char *) * (HISTORY_SIZE - 1));
        history[HISTORY_SIZE - 1] = strdup(cmd);
    }
}

void print_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}

// In main loop, add input command to history

// In main loop, add input command to history


void execute_builtin(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        chdir(args[1] ? args[1] : getenv("HOME"));
    } else if (strcmp(args[0], "exit") == 0) {