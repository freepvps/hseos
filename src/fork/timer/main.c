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
    if (argc < 3) {
        printf("Usage: %s seconds process [args...]\n", argv[0]);
        return -1;
    }
    unsigned seconds;
    if (sscanf(argv[1], "%u", &seconds) != 1) {
        printf("Parse seconds error\n");
        return -1;
    }

    uint64_t counter = 0;
    char counter_str[64];
    char* environ[2] = {
        counter_str,
        NULL,
    };

    while (1) {
        sleep(seconds);
        pid_t child = fork();
        if (child == 0) {
            sprintf(counter_str, "STEP=%" PRIu64, counter);
            int res = execve(argv[2], &argv[2], environ);
            printf("exec error: %s\n", strerror(errno));
            return -1;
        }
        ++counter;

        int status;
        pid_t res = waitpid(child, &status, 0);
        if (res == -1) {
            printf("wait child error: %s\n", strerror(errno));
        } else {
            if (WIFEXITED(status)) {
                printf("child exec success, ret code: %d\n", (int)WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("child signaled, signal: %d\n", (int)WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                printf("child stopped, signal: %d\n", (int)WSTOPSIG(status));
            } else {
                printf("invalid status, errno: %s\n", strerror(errno));
            }
        }
    }

    return 0;
}
