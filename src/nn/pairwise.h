#pragma once

#include "base.h"

typedef struct {
    operation_t op;
    const operation_t* inputs[2];
} pairwise_op_t;

typedef pairwise_op_t op_add_t;
typedef pairwise_op_t op_sub_t;
typedef pairwise_op_t op_mul_t;
typedef pairwise_op_t op_div_t;

operation_t* make_op_add(const operation_t* a, const operation_t* b);
operation_t* make_op_sub(const operation_t* a, const operation_t* b);
operation_t* make_op_mul(const operation_t* a, const operation_t* b);
operation_t* make_op_div(const operation_t* a, const operation_t* b);
