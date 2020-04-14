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
    int n;
    pid_t root_pid = getpid();
    check_result(scanf("%d", &n) != -1, "scanf");
    for (int i = 1; i <= n; i++) {
        printf("%d%c", i, i != n ? ' ' : '\n');
        fflush(stdout);
        pid_t pid = fork();
        check_result(pid != -1, "fork");
        if (pid) {
            break;
        }
    }
    wait(NULL);
    return 0;
}
