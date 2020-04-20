#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

volatile int received = 0;

void handler(int sig) {
    received = 1;
}

int main(int argc, char** argv) {
    signal(SIGTERM, &handler);

    while (1) {
        if (received) {
            received = 0;
            printf("received\n");
            while (sleep(5)) {
                printf("%s\n", strerror(errno));
            }
        }
    }
    return -1;
}
