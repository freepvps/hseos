#include "inputs.h"

#include <malloc.h>


static void compute_placeholder(operation_t* op) {
    op_placeholder_t* p = (op_placeholder_t*)op;
    p->op.output_value = *p->reference;
}
operation_t* make_op_placeholder(const float* reference) {
    static operation_vtable_t placeholder_vtable = {
        .compute = &compute_placeholder,
    };

    op_placeholder_t* p = (op_placeholder_t*)malloc(sizeof(*p));
    if (p == NULL) {
        return NULL;
    }
    p->reference = reference;
    p->op.inputs = NULL;
    p->op.inputs_count = 0;
    p->op.vtable = &placeholder_vtable;
    return (operation_t*)p;
}

static void compute_const(operation_t* op) {
    op_const_t* p = (op_const_t*)op;
    p->op.output_value = p->value;
}
operation_t* make_op_const(float value) {
    static operation_vtable_t const_vtable = {
        .compute = &compute_const,
    };

    op_const_t* p = (op_const_t*)malloc(sizeof(*p));
    if (p == NULL) {
        return NULL;
    }
    p->value = value;
    p->op.inputs = NULL;
    p->op.inputs_count = 0;
    p->op.vtable = &const_vtable;
    return (operation_t*)p;
}
