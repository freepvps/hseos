#pragma once

#include <stddef.h>

typedef struct operation operation_t;

typedef struct {
    void (*compute)(operation_t*);
} operation_vtable_t;

struct operation {
    size_t inputs_count;
    const operation_t** inputs;

    float output_value;

    struct {
        int output_computed;
    } computing;

    const operation_vtable_t* vtable;
};
