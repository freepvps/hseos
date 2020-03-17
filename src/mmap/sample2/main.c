#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

uint8_t from_hex_char(char c) {
    if (c >= '0' && c <= '9') {
        return (uint8_t)(c - '0');
    } else if (c >= 'A' && c <= 'F'){
        return (uint8_t)(10 + c - 'A');
    } else if (c >= 'a' && c <= 'f'){
        return (uint8_t)(10 + c - 'a');
    }
    return 0xFF;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        return -1;
    }
    char* code = argv[1];
    size_t size = strlen(code);

    char* ptr = mmap(
        NULL,
        size >> 1,
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    if (ptr == MAP_FAILED) {
        printf("%s\n", strerror(errno));
        return -1;
    }
    for (size_t i = 0; i < size; i += 2) {
        uint8_t v = (uint8_t)(
            (from_hex_char(code[i]) << 4)
            | from_hex_char(code[i + 1])
        );
        ptr[i >> 1] = (char)v;
    }
    int (*f_ptr)(int, int) = (int (*)(int, int))ptr;
    while (1) {
        int a, b;
        if (scanf("%d%d", &a, &b) != 2) {
            printf("%s\n", strerror(errno));
            return -1;
        }
        int res = f_ptr(a, b);
        printf("%d\n", res);
    }
    
    return 0;
}
