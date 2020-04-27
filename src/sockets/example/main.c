#include "types.h"

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int main(int argc, char** argv) {
    // ya.ru
    // http/https
    struct addrinfo* output;
    int err = getaddrinfo("localhost", "http", NULL, &output);
    if (err) {
        printf("%s\n", gai_strerror(err));
    }
    struct addrinfo* cur = output;
    while (cur) {
        struct sockaddr_in* addr = (struct sockaddr_in *)cur->ai_addr;
        printf("%s\n", inet_ntoa(addr->sin_addr));
        cur = cur->ai_next;
    }
    return 0;
}
