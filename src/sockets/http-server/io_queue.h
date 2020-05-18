#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

enum { IOQ_SEND = 1U, IOQ_RECV = 2U, IOQ_READY = 4 };

typedef struct {
    unsigned int in;
    unsigned int out; 
} io_flags_t;

struct io_list_node {
    io_flags_t flags;

    struct io_list_node* next;
    struct io_list_node* prev;
};

struct io_list {
    struct io_list_node zero;
};

static void io_list_init(struct io_list* list) {
    list->zero.next = &list->zero;
    list->zero.prev = &list->zero;
}

static struct io_list_node* io_list_begin(struct io_list* list) {
    return list->zero.next == &list->zero ? NULL : list->zero.next;
}

static void io_list_remove(struct io_list_node* node) {
    if (node->next) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->next = NULL;
        node->prev = NULL;
    }
}

static void io_list_push_back(struct io_list* list, struct io_list_node* node) {
    if (!node->next) {
        struct io_list_node* end = list->zero.prev;
        node->prev = end;
        node->next = end->next;
        end->next->prev = node;
        end->next = node;
    }
}

typedef struct {
    struct io_list_node* nodes;
    struct io_list ready_queue;
} io_queue_t;

io_queue_t* io_queue_create(size_t capacity);
void io_queue_free(io_queue_t*);

static io_flags_t* io_queue_get(io_queue_t* queue, size_t index) {
    return &queue->nodes[index].flags;
}

static void io_queue_wake(io_queue_t* queue, size_t index) {
    struct io_list_node* node = &queue->nodes[index];
    if (node->flags.in & node->flags.out) {
        io_list_push_back(&queue->ready_queue, node);
    } else {
        io_list_remove(node);
    }
}

static int io_queue_pop(io_queue_t* queue, size_t* index) {
    struct io_list_node* node = io_list_begin(&queue->ready_queue);
    if (node) {
        io_list_remove(node);
        *index = (node - queue->nodes);
        return 1;
    } else {
        return 0;
    }
}
