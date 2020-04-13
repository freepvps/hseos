#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <inttypes.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/eventfd.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>


void check_result(int flag, const char* msg) {
    if (!flag) {
        printf("%s error: %s\n", msg, strerror(errno));
    }
}

int ensure_write(int fd, const char* data, size_t size) {
    while (size) {
        ssize_t res = write(fd, data, size);
        if (res < 0) {
            return -1;
        }
        size -= (size_t)res;
        data += res;
    }
    return 0;
}

int main(int argc, char** argv) {
    int pipe_fd[2];
    check_result(pipe(pipe_fd) != -1, "pipe");

    pid_t child = fork();
    check_result(child != -1, "fork");
    if (child) {
        close(STDOUT_FILENO);
        check_result(dup2(pipe_fd[1], STDOUT_FILENO) - 1, "dup2");
    } else {
        close(STDIN_FILENO);
        check_result(dup2(pipe_fd[0], STDIN_FILENO) - 1, "dup2");
    }
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    char buf[1024];
    ssize_t size = 0;
    while ((size = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        if (ensure_write(STDOUT_FILENO, buf, (size_t)size) < 0) {
            break;
        }
    }

    return 0;
}
