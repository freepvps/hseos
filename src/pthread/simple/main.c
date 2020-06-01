#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_func(void* s) {
    static int test = 123;
    printf("thread\n");
    return &test;
}

int main() {
    printf("main\n");
    pthread_t th;
    pthread_create(&th, NULL, &thread_func, NULL);
    void* res;
    pthread_join(th, &res);
    printf("main %d\n", *(int*)res);

    return 0;
}
