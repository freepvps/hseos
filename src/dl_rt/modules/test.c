#include <stdio.h>

void _init() {
    printf("Initialize\n");
}

void _fini() {
    printf("Finalize\n");
}

int run(int a) {
    return a * a;
}
