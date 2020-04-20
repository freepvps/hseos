#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

typedef struct {
    sigset_t target_set;
    sigset_t old_set;
} signal_reader_t;

signal_reader_t sr_init(const sigset_t* s) {
    signal_reader_t res;
    res.target_set = *s;
    return res;
}

volatile int sr_exists = 0;
volatile int sr_last_signal;
void sr_sighandler(int sig) {
    sr_exists = 1;
    sr_last_signal = sig;
}

int sr_acquire(signal_reader_t* reader) {
    if (sigprocmask(SIG_BLOCK, &reader->target_set, &reader->old_set) == -1) {
        return -1;
    }
    for (int i = 1; i < _NSIG; i++) {
        if (sigismember(&reader->target_set, i)) {
            signal(i, &sr_sighandler);
        }
    }
    return 0;
}

int sr_release(signal_reader_t* reader) {
    sigprocmask(SIG_SETMASK, &reader->old_set, NULL);
    for (int i = 1; i < _NSIG; i++) {
        if (sigismember(&reader->target_set, i)) {
            signal(i, SIG_DFL);
        }
    }
    return 0;
}

int sr_read(signal_reader_t* reader) {
    while (!sr_exists) {
        sigsuspend(&reader->old_set);
    }
    sr_exists = 0;
    return sr_last_signal;
}
