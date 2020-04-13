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


enum {
    THREADS = 8,
    N = 10,
};

void check_result(int flag, const char* msg) {
    if (!flag) {
        printf("%s error: %s\n", msg, strerror(errno));
    }
}

int main(int argc, char** argv) {
    int fds[THREADS];
    pid_t children[THREADS];
    for (size_t i = 0; i < THREADS; i++) {
        check_result((fds[i] = eventfd(0, 0)) != -1, "efd");
    }

    size_t current_id = THREADS;
    for (size_t i = 0; i < THREADS; i++) {
        pid_t child = fork();
        check_result(child >= 0, "fork");
        if (!child) {
            current_id = i;
            break;
        }
    }
    if (current_id == THREADS) {
        uint64_t value = 1;
        check_result(write(fds[0], &value, sizeof(value)) == sizeof(value), "init write");
        
        while (wait(NULL) != -1) {}
    } else {
        while (1) {
            uint64_t x;
            check_result(read(fds[current_id], &x, sizeof(x)) == sizeof(x), "read efd");
            if (x <= N) {
                printf("%" PRIu64 "\n", x);
            }
            ++x;
            
            uint64_t next_id = (current_id + 1) % THREADS;
            check_result(write(fds[next_id], &x, sizeof(x)) == sizeof(x), "write efd");
            if (x > N) {
                exit(0);
            }
        }
    }

    return 0;
}
