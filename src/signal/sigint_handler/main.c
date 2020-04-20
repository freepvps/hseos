#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

void handler(int sig) {
    printf("%d\n", sig);
    for (int i = 0; i < 10; i++) {
        printf("handler\n");
        sleep(1);
    }
}

int main(int argc, char** argv) {
    signal(SIGTERM, &handler);
    
    int x = 1;
    int y;
    if (scanf("%d", &y) != 1) {
        return -1;
    }
    int z = x / y;
    printf("%d\n", z);

    while (1) {
        printf("alive!\n");
        sleep(1);
    }
    return -1;
}
