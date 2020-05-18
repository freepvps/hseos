#include "buffer.h"

#include <stdlib.h>
#include <memory.h>

int buffer_init(buffer_t* buffer, size_t capacity) {
    buffer->capacity = capacity;
    buffer->size = 0;
    buffer->memory = malloc(capacity);
    return buffer->memory == NULL ? -1 : 0;
}

int buffer_append(buffer_t* buffer, const void* data, size_t size) {
    if (buffer->size + size > buffer->capacity) {
        size_t n = buffer->capacity;
        while (buffer->size + size > n) {
            n <<= 1;
        }
        void* next_memory = realloc(buffer->memory, n);
        if (next_memory == NULL) {
            return -1;
        }
        buffer->capacity = n;
        buffer->memory = next_memory;
    }
    memcpy(((char*)buffer->memory) + buffer->size, data, size);
    buffer->size += size;
    return 0;
}

void buffer_consume(buffer_t* buffer, size_t size) {
    if (size > buffer->size) {
        buffer_clear(buffer);
        return;
    }
    buffer->size -= size;
    memmove(buffer->memory, ((char*)buffer->memory) + size, buffer->size);
}

void buffer_clear(buffer_t* buffer) {
    buffer->size = 0;
}

void buffer_free(buffer_t* buffer) {
    free(buffer->memory);
}
