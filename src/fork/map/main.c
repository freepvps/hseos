#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s process [args...]\n", argv[0]);
        return -1;
    }

    char* line = NULL;
    size_t size = 0;
    ssize_t res;
    while ((res = getline(&line, &size, stdin)) != -1) {
        size_t length = (size_t)res;
        while (length && line[length - 1] == '\n') {
            line[--length] = '\0';
        }
        pid_t pid = fork();
        if (pid == -1) {
            break;
        }
        if (pid == 0) {
            for (int i = 1; i < argc; i++) {
                if (strcmp(argv[i], "{}") == 0) {
                    argv[i] = line;
                }
            }
            execvp(argv[1], &argv[1]);
            printf("error: %s\n", strerror(errno));
            break;
        } else if (waitpid(pid, NULL, 0) == -1) {
            break;
        }
    }
    free(line);

    return 0;
}
