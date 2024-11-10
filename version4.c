#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

#define BUFFER_SIZE 1024
#define HISTORY_SIZE 10  // Maximum number of commands in history

char* history[HISTORY_SIZE];  // Array to store command history
int history_count = 0;        // Current count of commands in history

// Function to display the shell prompt with the current working directory
void display_prompt() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("PUCITshell@%s:- ", cwd);
    } else {
        perror("getcwd() error");
    }
}

// Function to add a command to history
void add_to_history(char* input) {
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(input);
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i - 1] = history[i];
        }
        history[HISTORY_SIZE - 1] = strdup(input);
    }
}

// Function to display command history
void display_history() {
    int start = history_count >= HISTORY_SIZE ? history_count - HISTORY_SIZE : 0;
    for (int i = start; i < history_count; i++) {
        printf("%d: %s", i - start + 1, history[i % HISTORY_SIZE]);
    }
}

// Function to retrieve command from history
char* get_command_from_history(int index) {
    if (index < 1 || index > history_count) {
        printf("No such command in history.\n");
        return NULL;
    }
    return strdup(history[(index - 1) % HISTORY_SIZE]);
}

// Function to read user input from the shell prompt
char* read_input() {
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t len = getline(&buffer, &bufsize, stdin);

    // Check if the user entered Ctrl+D (EOF)
    if (len == -1) {
        if (feof(stdin)) {  // End of input
            printf("\n");
            free(buffer);
            exit(0);  // Gracefully exit the shell
        }
    }
    return buffer;
}

