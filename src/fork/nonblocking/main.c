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

void check_result(int flag, const char* msg) {
    if (!flag) {
        printf("%s error: %s\n", msg, strerror(errno));
    }
}

int main(int argc, char** argv) {
    int fd[2];
    check_result(pipe2(fd, O_NONBLOCK) != -1, "pipe");

    pid_t pid = fork();
    check_result(pid != -1, "fork");

    if (pid) {
        close(fd[0]);
        while (1) {
            int x;
            scanf("%d", &x);
            write(fd[1], &x, sizeof(x));
        }
    } else {
        int x;
        ssize_t res;
        while ((res = read(fd[0], &x, sizeof(x))) != 0) {
            if (res == -1 || ENOBUFS) {
                printf("%d %s\n", (int)res, strerror(errno));
                sleep(1);
            } else {
                printf("value: %d\n", x);
            }
        }
    }
    
    return 0;
}
