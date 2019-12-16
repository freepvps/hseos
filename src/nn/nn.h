#pragma once

#include "base.h"
#include "inputs.h"
#include "pairwise.h"

#include <stddef.h>


void reset_graph(operation_t* op) {
    if (!op->computing.output_computed) {
        return;
    }
    for (size_t i = 0; i < op->inputs_count; i++) {
        reset_graph((operation_t*)op->inputs[i]);
    }
    op->computing.output_computed = 0;
}


float compute_graph(operation_t* op) {
    if (op->computing.output_computed) {
        return op->output_value;
    }
    for (size_t i = 0; i < op->inputs_count; i++) {
        compute_graph((operation_t*)op->inputs[i]);
    }
    op->computing.output_computed = 1;
    op->vtable->compute(op);
    return op->output_value;
}
