#include "sigreader.h"

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

int main(int argc, char** argv) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);

    signal_reader_t reader = sr_init(&set);
    sr_acquire(&reader);

    while (1) {
        int sig = sr_read(&reader);
        printf("%d\n", sig);
    }
    return -1;
}
