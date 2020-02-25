#include <stdio.h>


int call_write(int fd, const char* buf, size_t n) {
    int r;
    asm(
        "movl $4, %%eax\n\t"
        "movl %1, %%ebx\n\t"
        "movl %2, %%ecx\n\t"
        "movl %3, %%edx\n\t"
        "int $0x80\n\t"
        "movl %%eax, %0\n\t"
        : "=r"(r)
        : "r"(fd), "r"(buf), "r"(n)
        :
    );
    return r;
}


int main() {
    call_write(1, "Test\n", 5);
    return 0;
}
