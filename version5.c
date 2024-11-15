#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define HISTORY_SIZE 10
#define MAX_BG_PROCESSES 100

char* history[HISTORY_SIZE];
int history_count = 0;

pid_t bg_processes[MAX_BG_PROCESSES];
int bg_process_count = 0;

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
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s", i + 1, history[i]);
    }
}


// Built-in command: cd
void builtin_cd(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: expected argument\n");
    } else if (chdir(args[1]) != 0) {
        perror("cd");
    }
}

// Built-in command: jobs
void builtin_jobs() {
    printf("Background Jobs:\n");
    for (int i = 0; i < bg_process_count; i++) {
        if (bg_processes[i] != 0) {  // Only display active jobs
            printf("[%d] PID: %d\n", i + 1, bg_processes[i]);
        }
    }
}

// Built-in command: kill
void builtin_kill(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "kill: expected PID\n");
    } else {
        int pid = atoi(args[1]);
        int found = 0;

        // Search for the PID in the background process list
        for (int i = 0; i < bg_process_count; i++) {
            if (bg_processes[i] == pid) {
                found = 1;
                if (kill(pid, SIGKILL) == -1) {
                    perror("kill");
                } else {
                    printf("Process %d terminated.\n", pid);
                    bg_processes[i] = 0;  // Mark the process as removed
                }
                break;
            }
        }

        if (!found) {
            printf("kill: %d: no such process\n", pid);
        }
    }
}

// Built-in command: help
void builtin_help() {
    printf("Built-in commands:\n");
    printf("cd <directory>: Change the current directory\n");
    printf("exit: Exit the shell\n");
    printf("jobs: List background jobs\n");
    printf("kill <PID>: Kill a background process by PID\n");
    printf("history: Display command history\n");
    printf("help: Show this help message\n");
}
