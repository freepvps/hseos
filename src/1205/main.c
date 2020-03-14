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

struct Data {
    int16_t x;
    int64_t y;
};

void marshall(unsigned char *out, const struct Data *in);
void unmarshall(struct Data *out, const unsigned char *in);

ssize_t get_data_count(int fd);
int get_data(int fd, size_t pos, struct Data* out);
int set_data(int fd, size_t pos, const struct Data* in);
int modify(struct Data* tmp, int32_t a) {
    tmp->y += tmp->x * a;
    return 0;
}

int run(int fd, int32_t in_a) {
    ssize_t res = get_data_count(fd);
    if (res < 0) {
        return -1;
    }
    size_t size = (size_t)res;
    for (size_t i = 0; i < ((size + 1) >> 1); i++) {
        size_t j = size - i - 1;
        struct Data a, b;
        int status = (
            get_data(fd, i, &a) != -1
            && get_data(fd, j, &b) != -1
            && modify(&a, in_a) != -1
            && modify(&b, in_a) != -1
            && set_data(fd, i, &b) != -1
            && set_data(fd, j, &a) != -1
        );
        if (!status) {
            return -1;
        }
    }
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
