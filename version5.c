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
