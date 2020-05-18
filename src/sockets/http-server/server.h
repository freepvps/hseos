#pragma once

#include "io_queue.h"
#include "http.h"

#include <stddef.h>

typedef struct http_server http_server_t;

typedef void (*http_server_cb_t)(http_server_t* server, size_t index);

struct http_server {
    int ep_fd;
    io_queue_t* io_queue;

    http_context_t* contexts;
    http_server_cb_t* callbacks;

    io_queue_t* ready_requests;
    io_queue_t* finished;

    size_t* index_pool;
    size_t index_pool_size;
    size_t connections_limit;
};

int http_server_init(http_server_t* server, size_t max_connections, size_t buffer_capacity);
int http_server_listen(http_server_t* server, int fd);
size_t http_server_poll(http_server_t* server, http_context_t* contexts, size_t limit);
int http_server_sync(http_server_t* server, http_context_t* context);
void http_server_free(http_server_t* server);
