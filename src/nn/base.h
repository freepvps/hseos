#pragma once

#include <stddef.h>

typedef struct operation operation_t;

typedef struct {
    void (*compute)(operation_t*);
    operation_t* (*derivative)(operation_t*, const float*);
} operation_vtable_t;

struct operation {
    size_t inputs_count;
    operation_t** inputs;

    float output_value;
    int output_computed;

    const operation_vtable_t* vtable;
};
