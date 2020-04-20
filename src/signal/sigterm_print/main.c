#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

volatile int received = 0;

void handler(int sig) {
    received = 1;
    printf("sig: %d\n", sig);
}

int main(int argc, char** argv) {
    printf("%d\n", (int)getpid());

    signal(SIGTERM, &handler);

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGTERM);

    sigset_t old_set;

    sigprocmask(SIG_BLOCK, &set, &old_set);
    while (1) {
        sigsuspend(&old_set);
        if (received) {
            received = 0;
            printf("received\n");
            sleep(5);
            printf("%s\n", strerror(errno));
        }
    }
    return -1;
}
