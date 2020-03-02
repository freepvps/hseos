#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

enum { BUF_SIZE = 4096 };


struct FileContent {
    char* data;
    ssize_t size;
};

struct FileContent read_all(const char* path) {
    int f = -1;
    off_t end_offset = -1;
    int res = (
        (f = open(path, O_RDONLY)) != -1
        && (end_offset = lseek(f, 0, SEEK_END)) != -1
        && (lseek(f, 0, SEEK_SET) != -1)
    );
    struct FileContent ans = {NULL, -1};
    if (res) {
        res &= (
            (ans.data = malloc((size_t)end_offset + 1)) != NULL
        );
        ans.size = (size_t)end_offset;
    }
    if (res) {
        size_t offset = 0;
        while (res && offset < ans.size) {
            size_t to_read = ans.size - offset;
            ssize_t r = read(f, ans.data + offset, to_read);
            if (r == -1) {
                if (errno == EINTR) {
                    continue;
                }
                res = 0;
            } else if (!r) {
                res = 0;
            } else {
                offset += r;
            }
        }
        ans.data[ans.size] = '\0';
    }
    if (!res) {
        if (ans.data) {
            free(ans.data);
        }
        ans.data = NULL;
        ans.size = -1;
    }
    if (f != -1) {
        close(f);
    }
    return ans;
}

int main(int argc, char** argv) {
    struct FileContent data = read_all(argv[1]);
    if (data.data) {
        printf("%s", data.data); 
    } else {
        printf("Read error: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}
