#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <fcntl.h>

int generate_file(const char* path) {
    int f = open(path, O_WRONLY | O_CREAT, 0644);
    if (f == -1) {
        return -1;
    }
    for (int i = 0; i < 256; i++) {
        if (write(f, &i, sizeof(i)) != sizeof(i)) {
            close(f);
            return -1;
        }
    }
    close(f);
    return 0;
}

int merge(const char* path1, const char* path2) {
    int f[2] = {-1, -1};
    int peek[2] = {0};
    int r = (
        (f[0] = open(path1, O_RDONLY)) != -1
        && (f[1] = open(path1, O_RDONLY)) != -1
        && (read(f[0], &peek[0], sizeof(*peek)) == sizeof(*peek))
        && (read(f[1], &peek[1], sizeof(*peek)) == sizeof(*peek))
    );
    if (!r) {
        if (f[0] != -1) {
            close(f[0]);
        }
        if (f[1] != -1) {
            close(f[1]);
        }
        return r;
    }

    while (1) {
        ssize_t target = -1;
        for (size_t i = 0; i < sizeof(f) / sizeof(*f); i++) {
            if (f[i] == -1) {
                continue;
            }
            if (target == -1 || peek[target] > peek[i]) {
                target = (ssize_t)i;
            }
        }
        if (target == -1) {
            break;
        }
        printf("%d\n", peek[target]);
        if (read(f[target], &peek[target], sizeof(*peek)) != sizeof(*peek)) {
            close(f[target]);
            f[target] = -1;
        }
    }
    return 0;
}

int test_seek(const char* path) {
    int f = -1;
    off_t end_offset = -1;
    int res = (
        (f = open(path, O_RDONLY)) != -1
        && (end_offset = lseek(f, 0, SEEK_END)) != -1
        && (lseek(f, 0, SEEK_SET) != -1)
    );
    if (res) {
        size_t size = ((size_t)end_offset) / sizeof(int);
        for (size_t i = 0; i < size; i += 2) {
            int tmp;
            res &= (
                (!i || lseek(f, sizeof(tmp), SEEK_CUR) != -1)
                && read(f, &tmp, sizeof(tmp)) == sizeof(tmp)
            );
            printf("%d %d\n", (int)i, tmp);
        }
    }
    if (f != -1) {
        close(f);
    }
    return res ? 0 : -1;
}

int main() {
    return (
        generate_file("./a") != -1
        && generate_file("./b") != -1
        && printf("Test merge:\n") != -1
        && merge("./a", "./b") != -1
        && printf("Test seek:\n") != -1
        && test_seek("./a") != -1
    ) ? 0 : -1;
}
