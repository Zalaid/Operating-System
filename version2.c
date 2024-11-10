// myshellv2.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pwd.h>

#define MAX_INPUT 1024

void display_prompt() {
    char cwd[MAX_INPUT];
    char *username = getpwuid(getuid())->pw_name;

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("PUCITshell:%s@%s$ ", username, cwd);
    } else {
        perror("getcwd() error");
    }
}

void execute_redirect(char *args[], int input_fd, int output_fd) {
    if (input_fd != STDIN_FILENO) {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (output_fd != STDOUT_FILENO) {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
    execvp(args[0], args);
    perror("Execution failed");
    exit(EXIT_FAILURE);
}

void parse_and_execute(char *input) {
    char *args[MAX_INPUT / 2 + 1];
    int i = 0, input_fd = STDIN_FILENO, output_fd = STDOUT_FILENO;

    // Handle input and output redirection
    char *input_file = strstr(input, "<");
    char *output_file = strstr(input, ">");
    if (input_file) *input_file++ = '\0';
    if (output_file) *output_file++ = '\0';

    if (input_file) {
        input_fd = open(strtok(input_file, " "), O_RDONLY);
        if (input_fd < 0) {
            perror("Failed to open input file");
            return;
        }
    }
    if (output_file) {
        output_fd = open(strtok(output_file, " "), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0) {
            perror("Failed to open output file");
            if (input_fd != STDIN_FILENO) close(input_fd);
            return;
        }
    }

    // Parse command
    char *token = strtok(input, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    // Execute command with redirection
    pid_t pid = fork();
    if (pid == 0) {
        execute_redirect(args, input_fd, output_fd);
    } else {
        wait(NULL);
        if (input_fd != STDIN_FILENO) close(input_fd);
        if (output_fd != STDOUT_FILENO) close(output_fd);
    }
}

void execute_pipe(char *cmd1, char *cmd2) {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        parse_and_execute(cmd1);
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        close(fd[0]);
        parse_and_execute(cmd2);
        exit(EXIT_FAILURE);
    }

    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

void execute_command(char *input) {
    char *pipe_pos = strstr(input, "|");

    if (pipe_pos) {
        *pipe_pos = '\0';
        pipe_pos++;
        execute_pipe(input, pipe_pos);
    } else {
        parse_and_execute(input);
    }
}

int main() {
    char input[MAX_INPUT];

    while (1) {
        display_prompt();
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

