#include "nn.h"

#include <memory.h>
#include <stdio.h>

int main() {
    float value;

    operation_t* x = make_op_placeholder(&value);
    operation_t* xx = make_op_mul(x, x);

    operation_t* one = make_op_const(1);
    operation_t* xx1 = make_op_add(xx, one);

    operation_t* res = make_op_div(xx, xx1);

    value = 2;
    printf("%f\n", compute_graph(res));
    return 0;
}
