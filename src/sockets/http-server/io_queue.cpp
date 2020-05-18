#include "io_queue.h"

#include <stdlib.h>
#include <memory.h>

io_queue_t* io_queue_create(size_t capacity) {
    size_t alloc_size = sizeof(io_queue_t) + capacity * sizeof(io_list_node);
    io_queue_t* queue = (io_queue_t*)malloc(alloc_size);
    if (queue == NULL) {
        return NULL;
    }
    queue->nodes = (struct io_list_node*)(queue + 1);
    memset(queue, 0, alloc_size);
    return queue;
}

void io_queue_free(io_queue_t* queue) {
    free(queue);
}
