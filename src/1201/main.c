#include <endian.h>
#include <fcntl.h>
#include <memory.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int encode(int fd_out) {
    uint16_t x;
    while (scanf("%" SCNu16, &x) == 1) {
        x = htobe16(x);
        if (write(fd_out, &x, sizeof(x)) != sizeof(x)) {
            return -1;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    char* path_out = argv[1];

    int fd_out = -1;
    int res = (
        (fd_out = open(
            path_out,
            O_CREAT | O_WRONLY | O_TRUNC,
            0600
        )) != -1
    );
    if (res) {
        encode(fd_out);
    }
    if (fd_out != -1) {
        close(fd_out);
    }

    return 0;
}
