#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

struct AtomicList {
    struct AtomicList* next;
    void* payload;
};

typedef struct AtomicList atomic_list_t;


void atomic_push(atomic_list_t* head, atomic_list_t* node) {
    node->next = node;
    __atomic_exchange(&head->next, &node->next, &node->next, __ATOMIC_SEQ_CST);
}


int main() {
    atomic_list_t x = {NULL, NULL};
    atomic_list_t y = {NULL, NULL};
    atomic_push(&x, &y);

    printf("%d\n", x.next == &y);

    return 0;
}
