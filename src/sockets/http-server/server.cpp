#include "server.h"

#include <sys/epoll.h>

#define MALLOC_TYPED(type, count) (type*)malloc(sizeof(type) * count)

int http_server_init(http_server_t* server, size_t max_connections, size_t buffer_capacity) {
    int status = (
        (server->ep_fd = epoll_create((int)max_connections)) != -1
        && (server->io_queue = io_queue_create(max_connections))
        && (server->contexts = MALLOC_TYPED(http_context_t, max_connections))
        && (server->callbacks = MALLOC_TYPED(http_server_cb_t, max_connections))
        && (server->ready_requests = io_queue_create(max_connections))
        && (server->finished = io_queue_create(max_connections))
        && (server->index_pool = MALLOC_TYPED(size_t, max_connections))
    ) ? 0 : -1;
    if (status) {
        return status;
    }
    server->index_pool_size = max_connections;
    server->connections_limit = max_connections;
    for (size_t i = 0; i < server->index_pool_size; i++) {
        server->index_pool[i] = i;
        if (http_context_init(&server->contexts[i], buffer_capacity) == -1) {
            return -1;
        }
    }
    return 0;
}

int http_server_listen(http_server_t* server, int fd) {
    if (server->index_pool_size == 0) {
        return -1;
    }

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLERR | EPOLLET;
    ev.data.u64 = (uint64_t)server->connections_limit;
    if (epoll_ctl(server->ep_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        return -1;
    }
    --server->index_pool_size;
    return 0;
}

size_t http_server_poll(http_server_t* server, http_context_t* contexts, size_t limit);
int http_server_sync(http_server_t* server, http_context_t* context);
void http_server_free(http_server_t* server);
