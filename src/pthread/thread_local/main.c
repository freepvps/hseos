#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

struct State {
    size_t x;
    int cpu_id;
    void (*mutate)(size_t* x);
};

_Thread_local size_t tmp;

void mutate(size_t* x) {
    (*x)++;
}

void* thread_func(void* input) {
    struct State* x = (struct State*)input;

    printf("tmp: %d\n", (int)tmp);

    cpu_set_t s;
    CPU_ZERO(&s);
    CPU_SET(x->cpu_id, &s);
    pthread_setaffinity_np(pthread_self(), 1, &s);

    tmp = 0;
    for (size_t i = 0; i < 1000000000; i++) {
        x->mutate(&tmp);
    }
    x->x += tmp;

    return NULL;
}

int main() {
    tmp = 16;
    printf("main\n");
    printf("main tmp: %d\n", (int)tmp);
    pthread_t th1, th2;
    struct State s = {0, 0, &mutate};
    pthread_create(&th1, NULL, &thread_func, (void*)&s);
    s.cpu_id++;
    pthread_create(&th2, NULL, &thread_func, (void*)&s);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("main %d\n", (int)s.x);

    return 0;
}
