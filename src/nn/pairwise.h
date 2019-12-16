#pragma once

#include "base.h"

typedef struct {
    operation_t op;
    operation_t* inputs[2];
} pairwise_op_t;

typedef pairwise_op_t op_add_t;
typedef pairwise_op_t op_sub_t;
typedef pairwise_op_t op_mul_t;
typedef pairwise_op_t op_div_t;

operation_t* make_op_add(operation_t* a, operation_t* b);
operation_t* make_op_sub(operation_t* a, operation_t* b);
operation_t* make_op_mul(operation_t* a, operation_t* b);
operation_t* make_op_div(operation_t* a, operation_t* b);
