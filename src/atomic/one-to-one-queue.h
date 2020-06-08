#pragma once

#include <unistd.h>
#include <string.h>

typedef struct {
    size_t read_offset;
    size_t write_offset;
    size_t memory_size;
} atomic_queue_t;

inline atomic_queue_t* atomic_queue_init(void* buffer, size_t buffer_size) {
    if (buffer_size < sizeof(atomic_queue_t)) {
        return NULL;
    }
    atomic_queue_t* queue = (atomic_queue_t*)buffer;
    queue->read_offset = 0;
    queue->write_offset = 0;
    queue->memory_size = buffer_size - sizeof(atomic_queue_t);
    return queue;
}

inline int atomic_queue_try_enqueue(atomic_queue_t* queue, const char* data, size_t size) {
    if (size > queue->memory_size) {
        return -1;
    }
    if (!size) {
        return 0;
    }
    size_t w_off = queue->write_offset;
    size_t r_off = __atomic_load_n(&queue->read_offset, __ATOMIC_ACQUIRE);
    size_t free_space = (r_off + queue->memory_size - 1 - w_off) % queue->memory_size;

    if (size > free_space) {
        return -1;
    }

    char* queue_memory = ((char*)queue) + sizeof(*queue);
    size_t right_free = queue->memory_size - w_off;
    if (size >= right_free) {
        memcpy(&queue_memory[w_off], data, right_free);
        w_off = 0;
        data += right_free;
        size -= right_free;
    }
    memcpy(&queue_memory[w_off], data, size);
    w_off += size;

    __atomic_store_n(&queue->write_offset, w_off, __ATOMIC_RELEASE);
    return 0;
}

inline int atomic_queue_try_dequeue(atomic_queue_t* queue, char* data, size_t size) {
    if (size > queue->memory_size) {
        return -1;
    }
    if (!size) {
        return 0;
    }
    size_t w_off = __atomic_load_n(&queue->write_offset, __ATOMIC_ACQUIRE);
    size_t r_off = queue->read_offset;
    size_t available_data = (w_off + queue->memory_size - r_off) % queue->memory_size;

    if (size > available_data) {
        return -1;
    }

    char* queue_memory = ((char*)queue) + sizeof(*queue);
    size_t right_free = queue->memory_size - r_off;
    if (size >= right_free) {
        memcpy(data, &queue_memory[r_off], right_free);
        r_off = 0;
        data += right_free;
        size -= right_free;
    }
    memcpy(data, &queue_memory[r_off], size);
    r_off += size;

    __atomic_store_n(&queue->read_offset, r_off, __ATOMIC_RELEASE);
    return 0;
}
