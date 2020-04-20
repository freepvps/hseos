#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

enum { CHILDREN_COUNT = 4, LIMIT = 100 };

volatile int received = 0;

void handler(int x) {
    received = 1;
}

void wait_signal() {
    while (!received) {
        pause();
    }
    received = 0;
}

int main(int argc, char** argv) {
    int* mem = (int*)mmap(
        NULL,
        sizeof(*mem),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );
    if (mem == MAP_FAILED) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    *mem = 0;

    signal(SIGUSR1, &handler);

    int is_child = 0;
    pid_t children[CHILDREN_COUNT];
    for (size_t i = 0; i < CHILDREN_COUNT; i++) {
        children[i] = fork();
        if (children[i] == -1) {
            exit(-1);
        }
        if (is_child = (children[i] == 0)) {
            break;
        }
    }

    if (is_child) {
        pid_t parent = getppid();
        while (1) {
            wait_signal();
            int value = (*mem)++;
            printf("%d\n", value);
            kill(parent, SIGUSR1);
        }
    } else {
        size_t index = 0;
        raise(SIGUSR1);

        while (1) {
            wait_signal();
            size_t next = (index++) % CHILDREN_COUNT;
            if (*mem < LIMIT) {
                kill(children[next], SIGUSR1);
            } else {
                break;
            }
        }
        for (size_t i = 0; i < CHILDREN_COUNT; i++) {
            kill(children[i], SIGKILL);
        }
    }

    return 0;
}
