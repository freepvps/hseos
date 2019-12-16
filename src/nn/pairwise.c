#include "pairwise.h"

#include <malloc.h>


static operation_t* make_op_pairwise(const operation_t* a, const operation_t* b, const operation_vtable_t* vtable) {
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
operation_t* make_op_add(const operation_t* a, const operation_t* b) {
    static operation_vtable_t vtable = {
        .compute = &compute_add,
    };
    return make_op_pairwise(a, b, &vtable);
}

static void compute_sub(operation_t* op) {
    float a = op->inputs[0]->output_value;
    float b = op->inputs[1]->output_value;
    op->output_value = a - b;
}
operation_t* make_op_sub(const operation_t* a, const operation_t* b) {
    static operation_vtable_t vtable = {
        .compute = &compute_sub,
    };
    return make_op_pairwise(a, b, &vtable);
}

static void compute_mul(operation_t* op) {
    float a = op->inputs[0]->output_value;
    float b = op->inputs[1]->output_value;
    op->output_value = a * b;
}
operation_t* make_op_mul(const operation_t* a, const operation_t* b) {
    static operation_vtable_t vtable = {
        .compute = &compute_mul,
    };
    return make_op_pairwise(a, b, &vtable);
}

static void compute_div(operation_t* op) {
    float a = op->inputs[0]->output_value;
    float b = op->inputs[1]->output_value;
    op->output_value = a / b;
}
operation_t* make_op_div(const operation_t* a, const operation_t* b) {
    static operation_vtable_t vtable = {
        .compute = &compute_div,
    };
    return make_op_pairwise(a, b, &vtable);
}
