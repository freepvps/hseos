#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int check_socket_result(ssize_t res) {
    if (res == -1 && errno != EINTR) {
        return -1;
    }
    if (res == 0) {
        return -1;
    }
    return 0;
}

int main(int argc, char** argv) {
    // ya.ru
    // http/https
    struct addrinfo* output;
    int err = getaddrinfo("beast.gvg.pw", "25000", NULL, &output);
    if (err) {
        printf("%s\n", gai_strerror(err));
    }
    struct sockaddr_in* addr = (struct sockaddr_in *)output->ai_addr;

    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(client, addr, sizeof(*addr)) == -1) {
        printf("connect error: %s\n", strerror(errno));
        return -1;
    }
    char data[1024];
    while (scanf("%s", data) == 1) {
        size_t len = strlen(data);
        write(client, data, len);
        ssize_t recv_size = read(client, data, len);
        data[recv_size] = '\0';
        printf("%s\n", data);
    }

    return 0;
}
