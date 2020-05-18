#pragma once

#include "buffer.h"
#include "picohttpparser/picohttpparser.h"

enum { HTTP_HEADERS_LIMIT = 64 };

typedef struct {
    buffer_t buffer;

    const char* method;
    size_t method_len;

    const char* path;
    size_t path_len;

    int minor_version;
    struct phr_header headers[HTTP_HEADERS_LIMIT];
    size_t num_headers;
} http_request_t;

typedef struct {
    buffer_t buffer;
} http_response_t;

typedef struct {
    int status;  // -1 - broken, 0 - waiting data, >0 - handling request
    http_request_t request;
    http_response_t response;
} http_context_t;

int http_context_init(http_context_t* context, size_t capacity);
int http_context_handle_data(http_context_t* context, const void* data, size_t size);
void http_context_clear(http_context_t* context);
void http_context_free(http_context_t* context);

int http_write_data(http_context_t* context, const void* data, size_t size);
void http_finish(http_context_t* context);
