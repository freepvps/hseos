#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <memory.h>
#include <stdint.h>


void magic(int x[4]) {
    printf("%d %d %d %d\n", x[0], x[1], x[2], x[3]);
}

int main() {
    int x[4] = {1, 2};
    magic(x);
    int y[2][2] = {{1, 2}, {3, 4}};
    magic(y);  // just warn
    
    int* t = &y[0][0];
    magic(t);
    return 0;
}
