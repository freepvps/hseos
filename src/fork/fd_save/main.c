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
    int fd = open("./data.txt", O_RDONLY);
    pid_t pid = fork();

    if (pid == 0) {
        char fd_str[32];
        char* new_args[3] = {
            "./test",
            fd_str,
            NULL
        };
        sprintf(fd_str, "%d", fd);
        execv("./test", new_args);
    } else if (pid > 0) {
        close(fd);
    } else {
        printf("error: %d %d\n", (int)getpid(), (int)getppid());
    }

    while (wait(NULL) != -1) {}

    return 0;
}
