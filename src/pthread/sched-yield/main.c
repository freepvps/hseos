#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <x86intrin.h>


size_t rdtscp() {
    int tmp;
    return (size_t)__rdtscp(&tmp);
}

int main() {
    pid_t pid = fork();

    cpu_set_t s;
    CPU_ZERO(&s);
    CPU_SET(0, &s);
    pthread_setaffinity_np(pthread_self(), 1, &s);


    while (1) {
        if (pid) {
            size_t ticks = rdtscp();
            sched_yield();
            size_t delta = rdtscp() - ticks;
            if (delta > 10000) {
                printf("%d\n", (int)delta);
            }
        }
    }

    return 0;
}
