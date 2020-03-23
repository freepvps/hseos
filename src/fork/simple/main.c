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
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char** argv) {
    pid_t pid = fork();
    size_t counter = 0;
    while (1) {
        if (pid == 0) {
            printf("child: %d %d\n", (int)getpid(), (int)getppid());
        } else if (pid > 0) {
            printf("parent:  %d %d\n", (int)getpid(), (int)getppid());
        } else {
            printf("error: %d %d\n", (int)getpid(), (int)getppid());
            break;
        }
        if ((++counter) % 5 == 0) {
            fflush(stdout);
        }
        sleep(1);
    }


    return 0;
}
