#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

struct State {
    size_t x;
    void (*mutate)(size_t* x);
};

void mutate(size_t* x) {
    (*x)++;
}

void* thread_func(void* input) {
    cpu_set_t s;
    CPU_ZERO(&s);
    CPU_SET(0, &s);
    pthread_setaffinity_np(pthread_self(), 1, &s);

    struct State* x = (struct State*)input;
    for (size_t i = 0; i < 1000000000; i++) {
        x->mutate(&x->x);
    }

    return NULL;
}

int main() {
    printf("main\n");
    pthread_t th1, th2;
    struct State s = {0, &mutate};
    pthread_create(&th1, NULL, &thread_func, (void*)&s);
    pthread_create(&th2, NULL, &thread_func, (void*)&s);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("main %d\n", (int)s.x);

    return 0;
}
