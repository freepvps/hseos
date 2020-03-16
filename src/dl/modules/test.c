#include <stdio.h>

void _init() {
    printf("Initialize\n");
}

void _fini() {
    printf("Finalize\n");
}


void run() {
    printf("Run\n");
}
