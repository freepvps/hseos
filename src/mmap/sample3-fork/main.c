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

    pid_t child = fork();
    if (child) {
        printf("parent: %d\n", (int)getpid());
        int last_value = 0;
        while (1) {
            int new_value = *mem;
            if (last_value != new_value) {
                printf("update: %d\n", new_value);
                last_value = new_value;
            }
            sleep(1);
        }
    } else {
        printf("child: %d\n", (int)getpid());
        while (scanf("%d", mem) == 1) {
            msync(mem, sizeof(*mem), MS_ASYNC);
        }
    }

    return 0;
}
