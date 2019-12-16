#pragma once

#include "base.h"

typedef struct {
    operation_t op;
    const float* reference;
} op_placeholder_t;

typedef struct {
    operation_t op;
    float value;
} op_const_t;

operation_t* make_op_placeholder(const float* reference);
operation_t* make_op_const(float value);
