#include <asm/unistd_32.h>
#include <unistd.h>
#include <stddef.h>

enum { BUF_SIZE = 4096 };

int read_call(int fd, char* buf, size_t n) {
    volatile int readed = 0;
    asm volatile(
        "movl    %1, %%eax\n"
        "movl    %2, %%ebx\n"
        "movl    %3, %%ecx\n"
        "movl    %4, %%edx\n"
        "int     $0x80\n"
        "movl    %%eax, %0\n"
        : "=g" (readed)
        : "g" (__NR_read), "g" (fd), "g" (buf), "g" (n)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    return readed;
}

int write_call(int fd, const char* buf, size_t n) {
    volatile int readed = 0;
    asm volatile(
        "movl    %1, %%eax\n"
        "movl    %2, %%ebx\n"
        "movl    %3, %%ecx\n"
        "movl    %4, %%edx\n"
        "int     $0x80\n"
        "movl    %%eax, %0\n"
        : "=g" (readed)
        : "g" (__NR_write), "g" (fd), "g" (buf), "g" (n)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    return readed;
}

void exit_call(int code) {
    asm volatile(
        "movl    %0, %%eax\n"
        "movl    %1, %%ebx\n"
        "int     $0x80\n"
        :
        : "g" (__NR_exit), "g" (code)
        : "%eax", "%ebx"
    );
}

struct FileReadState {
    int fd;
    unsigned char *buf;
    int bufsize;
    int lc;
    int read_offset;
    int size;
};

static char buffer[BUF_SIZE];
static struct FileReadState stin_state = {
    0,
    buffer,
    BUF_SIZE,
    -1,
    0,
    0,
};
struct FileReadState* stin = &stin_state;

int lastchar() {
    return stin->lc;
}

int nextchar_from_buffer() {
    if (stin->read_offset < stin->size) {
        stin->lc = stin->buf[stin->read_offset++];
        return lastchar();
    }
    return -1;
}

int nextchar() {
    int res = nextchar_from_buffer();
    if (res != -1) {
        return res;
    }
    stin->read_offset = stin->size = 0;
    res = read_call(stin->fd, stin->buf, stin->bufsize);
    if (res <= 0) {
        stin->lc = -1;
        return -1;
    } else {
        stin->size = res;
        return nextchar_from_buffer();
    }
}

void _start() {
    int ch = nextchar();
    if (ch == -1) {
        exit_call(-1);
    }
    write_call(1, (char*)&ch, 1);
    exit_call(0);
}
