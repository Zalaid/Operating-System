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
