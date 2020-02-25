#include <stdio.h>
#include <unistd.h>


int main() {
    char c;
    int res = read(0, &c, 1);
    printf("res: %d\n", res);
    return 0;
}
