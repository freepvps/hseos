#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

/*
  let result = 0;
  let shift = 0;
  while (true) {
    const byte = input.shift();
    result |= (byte & 0x7f) << shift;
    shift += 7;
    if ((0x80 & byte) === 0) {
      if (shift < 32 && (byte & 0x40) !== 0) {
        return result | (~0 << shift);
      }
      return result;
    }
  }*/

enum {
    LEB_ZERO = 0,
    LEB_PARTIAL = 1,
    LEB_FINISHED = 2,
};

struct Parser {
    uint32_t result;
    int shift;
    int state;
};

int put_char(struct Parser* parser, char c, int32_t* res) {
    if (parser->state == LEB_FINISHED) {
        parser->state = LEB_ZERO;
        parser->shift = 0;
        parser->result = 0;
    }
    parser->state = LEB_PARTIAL;
    parser->result |= ((uint32_t)(c & 0x7f)) << parser->shift;
    parser->shift += 7;
    if ((0x80 & c) == 0) {
        if (parser->shift < 32 && (c & 0x40) != 0) {
            parser->result = parser->result | (~0 << parser->shift);
        }
        *res = (int32_t)parser->result;
        parser->state = LEB_FINISHED;
        return 0;
    }
    return -2;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        return -1;
    }

    struct stat s;
    int fd;
    int res = (
        stat(argv[1], &s) != -1
        && (fd = open(argv[1], O_RDONLY)) != -1
    );

    int page_size = getpagesize();
    printf("Page size: %d\n", page_size);
    size_t block_size = 0;
    while (block_size < s.st_size) {
        size_t next_block_size = block_size + (size_t)page_size;
        if (next_block_size > s.st_size) {
            next_block_size = s.st_size;
        }
        char* tmp = mmap(
            NULL,
            next_block_size,
            PROT_READ,
            MAP_SHARED,
            fd,
            0
        );
        if (tmp == MAP_FAILED) {
            break;
        } else {
            munmap(tmp, next_block_size);
            block_size = next_block_size;
        }
    }

    struct Parser parser = {};
    int32_t value;
    for (size_t sz = 0; sz < s.st_size; sz += block_size) {
        size_t limit = s.st_size - sz;
        if (limit < block_size) {
            block_size = limit;
        }
        char* mem = mmap(
            NULL,
            block_size,
            PROT_READ,
            MAP_SHARED,
            fd,
            sz
        );
        for (size_t i = 0; i < s.st_size; i++) {
            if (!put_char(&parser, mem[i], &value)) {
                printf("%d\n", value);
            }
        }
        munmap(mem, block_size);
    }
    return 0;
}
