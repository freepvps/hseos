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


typedef struct {
    char* data;
    size_t size;
} buffer_t;


void check_result(int flag, const char* msg) {
    if (!flag) {
        printf("%s error: %s\n", msg, strerror(errno));
    }
}

buffer_t run(char** argv) {
    int fd[2];
    check_result(pipe(fd) != -1, "pipe");
    pid_t pid = fork();
    check_result(pid != -1, "fork");

    if (!pid) {
        close(STDOUT_FILENO);
        check_result(dup2(fd[1], STDOUT_FILENO) != -1, "dup2");
        close(fd[0]);
        close(fd[1]);
        execv(argv[0], argv);
    } else {
        close(fd[1]);
    }

    size_t capacity = 4096;
    buffer_t output;
    output.data = malloc(capacity);
    output.size = 0;

    check_result(output.data != NULL, "malloc");

    char data[4096];
    ssize_t res;
    while ((res = read(fd[0], data, sizeof(data))) > 0) {
        size_t n = (size_t)res;
        if (n + output.size > capacity) {
            while (n + output.size > capacity) {
                capacity <<= 1;
            }
            output.data = realloc(output.data, capacity);
            check_result(output.data != NULL, "realloc");
        }
        memcpy(output.data + output.size, data, n);
        output.size += n;
    }
    wait(NULL);
    return output;
}

int main(int argc, char** argv) {
    char* args[] = {
        "/bin/ls",
        NULL,
    };
    buffer_t buf = run(args);
    return write(STDOUT_FILENO, buf.data, buf.size) > 0 ? 0 : -1;
}
