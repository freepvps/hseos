#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <inttypes.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/eventfd.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>


enum {
    MAX_RESPONSES = 64,
    THREADS = 8,
    SEC_MICROSECONDS = 1000000,
};


struct Context {
    size_t response_size;
    uint64_t responses[MAX_RESPONSES];
};

void check_result(int flag, const char* msg) {
    if (!flag) {
        printf("%s error: %s\n", msg, strerror(errno));
    }
}

void loop_fd(int input_fd, int output_fd, size_t id, struct Context* ctx) {
    uint64_t value;
    while (1) {
        check_result(read(input_fd, &value, sizeof(value)) == sizeof(value), "read");

        if (value == 0) {
            printf("zero\n");
            continue;
        }
        uint64_t sqrt_part = (uint64_t)(sqrtl((long double)value) + 1);
        uint64_t step = (sqrt_part + THREADS - 1) / THREADS;

        uint64_t l = step * id;
        uint64_t r = step * (id + 1);
        l = l < 2 ? 2 : l;

        for (uint64_t i = l; i < r && i * i <= value; i++) {
            if (value % i == 0) {
                uint64_t j = value / i;
                ctx->responses[__sync_fetch_and_add(&ctx->response_size, 1)] = i;
                ctx->responses[__sync_fetch_and_add(&ctx->response_size, 1)] = j;
            }
        }

        check_result(write(output_fd, &value, sizeof(value)) == sizeof(value), "write");
    }
}


int main(int argc, char** argv) {
    struct Context* ctx = (struct Context*)mmap(
        NULL,
        sizeof(*ctx),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );
    int input_fd[THREADS];
    int output_fd[THREADS];
    pid_t children[THREADS];
    for (size_t i = 0; i < THREADS; i++) {
        check_result((input_fd[i] = eventfd(0, 0)) != -1, "efd");
        check_result((output_fd[i] = eventfd(0, 0)) != -1, "efd");
        check_result((children[i] = fork()) != -1, "fork");
        if (children[i] == 0) {
            close(STDIN_FILENO);
            loop_fd(input_fd[i], output_fd[i], i, ctx);
        }
    }

    while (1) {
        uint64_t x = 0;
        check_result(scanf("%" SCNu64, &x) > 0, "scanf");
        struct timeval begin;
        gettimeofday(&begin, NULL);

        ctx->response_size = 0;
        for (size_t i = 0; i < THREADS; i++) {
            check_result(write(input_fd[i], &x, sizeof(x)) == sizeof(x), "write req");
        }
        for (size_t i = 0; i < THREADS; i++) {
            check_result(read(output_fd[i], &x, sizeof(x)) == sizeof(x), "read resp");
        }
        struct timeval end;
        gettimeofday(&end, NULL);
        uint64_t delta = (end.tv_sec - begin.tv_sec) * SEC_MICROSECONDS + (end.tv_usec - begin.tv_usec);

        printf("responses: %" PRIu64 " time us: %" PRIu64 "\n", ctx->response_size, delta);
        for (size_t i = 0; i < ctx->response_size; i++) {
            printf("[%" PRIu64 "] = %" PRIu64  "\n", (uint64_t)i, ctx->responses[i]);
        }
    }

    return 0;
}
