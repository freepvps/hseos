#include "pairwise.h"

#include <malloc.h>


static operation_t* make_op_pairwise(operation_t* a, operation_t* b, const operation_vtable_t* vtable) {
    pairwise_op_t* p = (pairwise_op_t*)malloc(sizeof(*p));
    if (p == NULL) {
        return NULL;
    }
    p->inputs[0] = a;
    p->inputs[1] = b;
    p->op.inputs = &p->inputs[0];
    p->op.inputs_count = 2;
    p->op.vtable = vtable;
    return (operation_t*)p;
}


static void compute_add(operation_t* op) {
    float a = op->inputs[0]->output_value;
    float b = op->inputs[1]->output_value;
    op->output_value = a + b;
}
static operation_t* derivative_add(operation_t* op, const float* v) {
    operation_t* d1 = op->inputs[0]->vtable->derivative(op->inputs[0], v);
    operation_t* d2 = op->inputs[1]->vtable->derivative(op->inputs[1], v);
    return make_op_add(d1, d2);
}
operation_t* make_op_add(operation_t* a, operation_t* b) {
    static operation_vtable_t vtable = {
        .compute = &compute_add,
        .derivative = &derivative_add,
    };
    return make_op_pairwise(a, b, &vtable);
}

static void compute_sub(operation_t* op) {
    float a = op->inputs[0]->output_value;
    float b = op->inputs[1]->output_value;
    op->output_value = a - b;
}
static operation_t* derivative_sub(operation_t* op, const float* v) {
    operation_t* d1 = op->inputs[0]->vtable->derivative(op->inputs[0], v);
    operation_t* d2 = op->inputs[1]->vtable->derivative(op->inputs[1], v);
    return make_op_sub(d1, d2);
}
operation_t* make_op_sub(operation_t* a, operation_t* b) {
    static operation_vtable_t vtable = {
        .compute = &compute_sub,
        .derivative = &derivative_sub,
    };
    return make_op_pairwise(a, b, &vtable);
}

static void compute_mul(operation_t* op) {
    float a = op->inputs[0]->output_value;
    float b = op->inputs[1]->output_value;
    op->output_value = a * b;
}
static operation_t* derivative_mul(operation_t* op, const float* v) {
    operation_t* d1 = op->inputs[0]->vtable->derivative(op->inputs[0], v);
    operation_t* d2 = op->inputs[1]->vtable->derivative(op->inputs[1], v);
    return make_op_add(
        make_op_mul(d1, op->inputs[1]),
        make_op_mul(d2, op->inputs[0])
    );
}
operation_t* make_op_mul(operation_t* a, operation_t* b) {
    static operation_vtable_t vtable = {
        .compute = &compute_mul,
        .derivative = &derivative_mul,
    };
    return make_op_pairwise(a, b, &vtable);
}

static void compute_div(operation_t* op) {
    float a = op->inputs[0]->output_value;
    float b = op->inputs[1]->output_value;
    op->output_value = a / b;
}
static operation_t* derivative_div(operation_t* op, const float* v) {
    operation_t* d1 = op->inputs[0]->vtable->derivative(op->inputs[0], v);
    operation_t* d2 = op->inputs[1]->vtable->derivative(op->inputs[1], v);
    operation_t* a = make_op_sub(
        make_op_mul(d1, op->inputs[1]),
        make_op_mul(d2, op->inputs[0])
    );
    operation_t* b = make_op_mul(op->inputs[1], op->inputs[1]);
    return make_op_div(a, b);
}
operation_t* make_op_div(operation_t* a, operation_t* b) {
    static operation_vtable_t vtable = {
        .compute = &compute_div,
        .derivative = &derivative_div,
    };
    return make_op_pairwise(a, b, &vtable);
}
