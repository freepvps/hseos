#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

struct __attribute__((__packed__))  S {
    uint8_t x1;
    uint8_t x2;
    uint32_t ui32;
    uint64_t ui64;
};

int main() {
    struct S s = {0, 1, 2, 3};
    char* a = (void*)&s.x1;
    char* b = (void*)&s.ui32;
    printf("%" PRIx64 "\n", (uint64_t)a);
    printf("%" PRIx64 "\n", (uint64_t)b);
    printf("%" PRIu64 "\n", sizeof(struct S));
    return 0;
}
