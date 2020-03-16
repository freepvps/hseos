#include <stdio.h>
#include <dlfcn.h>

typedef int (*read_t)(int, char*, size_t);

int read(int fd, char* buf, size_t n) {
    static read_t real_read = NULL;
    if (real_read == NULL) {
        real_read = (read_t)dlsym(((void *) -1l), "read");
    }
    printf("read!\n");
    return real_read(fd, buf, n);
}
