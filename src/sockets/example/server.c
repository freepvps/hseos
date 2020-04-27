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
    const char* bind_addr = "95.217.13.136:25000";
    eo_addr_t addr;
    eo_parse_addr_port(bind_addr, &addr);

    int server = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    setsockopt(server, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));

    if (bind(server, &addr.base, eo_addr_size(&addr)) == -1) {
        printf("%s\n", strerror(errno));
            return -1;
    }

    listen(server, 128);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int connection = accept(server, &client_addr, &client_addr_len);
        pid_t proc = fork();
        if (proc == -1) {
            return -1;
        }
        if (!proc) {
            write(connection, "HTTP/1.1 200 OK\nServer: Fake\nContent-Language: ru\nContent-Type: text/html; charset=utf-8\n\n", 91);
            char buf[1024];
            while (1) {
                ssize_t recv_size = recv(connection, buf, sizeof(buf), 0);
                if (check_socket_result(recv_size) == -1) {
                    break;
                }
                ssize_t sent_size = send(connection, buf, (size_t)recv_size, 0);
                if (check_socket_result(recv_size) == -1) {
                    break;
                }
            }
            close(connection);
            return 0;
        }
    }
    return 0;
}
