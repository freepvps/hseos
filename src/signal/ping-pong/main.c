#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/signalfd.h>


void check_result(int status, const char* msg) {
    if (!status) {
        printf("%s %s\n", msg, strerror(errno));
        exit(-1);
    }
}

int read_number(int fd, uint64_t* res) {
    char s[16];
    ssize_t t = read(fd, s, sizeof(s));
    if (t != sizeof(s)) {
        return -1;
    }
    int status = sscanf(s, "%" SCNu64 "\n", res) == 1 ? 0 : -1;
    return status;
}

int write_number(int fd, uint64_t v) {
    char s[16];
    if (sprintf(s, "%" PRIu64 "\n", v) == -1) {
        return -1;
    }
    if (write(fd, s, sizeof(s)) != sizeof(s)) {
        return -1;
    }
    return 0;
}

void run_child(int sigfd, uint64_t n, int r, int w, pid_t pid) {
    int is_first = 0;
    if (pid == 0) {
        uint64_t t;
        check_result(read_number(r, &t) != -1, "wait pid");

        pid = (pid_t)t;
        is_first = 1;
    }
    while (1) {
        struct signalfd_siginfo sig;
        if (read(sigfd, &sig, sizeof(sig)) == -1) {
            continue;
        }
        uint64_t v;
        read_number(r, &v);
        if (v <= n) {
            printf("%d %" PRIu64 "\n", is_first ? 1 : 2, v);
            fflush(stdout);
        }
        kill(pid, SIGUSR1);
        write_number(w, v + 1);
        if (v >= n) {
            exit(0);
        }
    }
}


int main(int argc, char** argv) {
    size_t n = (size_t)atoi(argv[1]);
    int pipe_fd[2];
    check_result(pipe(pipe_fd) != -1, "create pipe");

    sigset_t sig_mask;
    sigemptyset(&sig_mask);
    sigaddset(&sig_mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sig_mask, NULL);
    int sig_fd = signalfd(-1, &sig_mask, 0);

    pid_t child1 = fork();
    check_result(child1 != -1, "fork child1");
    if (child1 == 0) {
        run_child(sig_fd, n, pipe_fd[0], pipe_fd[1], 0);
        return 0;
    }

    pid_t child2 = fork();
    check_result(child1 != -1, "fork child1");
    if (child2 == 0) {
        run_child(sig_fd, n, pipe_fd[0], pipe_fd[1], child1);
        return 0;
    }

    check_result(write_number(pipe_fd[1], (uint64_t)child2) != -1, "init child1");
    check_result(write_number(pipe_fd[1], 1) != -1, "init sequence");
    kill(child1, SIGUSR1);

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    while (wait(NULL) != -1) {
    }

    return 0;
}
