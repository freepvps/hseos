#include <stdio.h>


int main() {
    int y = 12;
    int x;
    asm(
        "movl $15, %%eax\n\t"
        "addl %%eax, %%ecx\n\t"
        "movl %%ecx, %0\n\t"
        : "=r"(x)
        : "c"(y)
        : "%eax"
    );
    printf("x: %d\n", x);
    return 0;
}