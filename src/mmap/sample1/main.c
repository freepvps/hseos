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


int mem_rw_loop(char* mem) {
    int* value = (int*)mem;
    while (1) {
        int x;
        if (scanf("%d", &x) != 1) {
            return -1;
        }
        if (x == 0) {
            printf("%d\n", *value);
        } else {
            *value = x;
            int res = msync(mem, sizeof(*value), MS_SYNC);
            if (res == -1) {
                printf("%s\n", strerror(errno));
            }
        }
    }
}


int main(int argc, char** argv) {
    if (argc != 2) {
        return -1;
    }
    struct stat s;
    int fd;
    char* mem;

    int res = (
        stat(argv[1], &s) != -1
        && (fd = open(argv[1], O_RDWR)) != -1
        && (mem = (char*)mmap(
            NULL,
            s.st_size,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            fd,
            0
        )) != MAP_FAILED
    );
    if (!res) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }

    mem_rw_loop(mem);

    munmap(mem, s.st_size);
    return 0;
}
