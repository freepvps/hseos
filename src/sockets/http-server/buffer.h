#pragma once

#include <stddef.h>

typedef struct {
    size_t capacity;
    size_t size;
    void* memory;
} buffer_t;

int buffer_init(buffer_t* buffer, size_t capacity);
int buffer_append(buffer_t* buffer, const void* data, size_t size);
void buffer_consume(buffer_t* buffer, size_t size);
void buffer_clear(buffer_t* buffer);
void buffer_free(buffer_t* buffer);
