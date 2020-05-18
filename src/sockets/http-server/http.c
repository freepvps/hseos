#include "http.h"

int http_context_init(http_context_t* context, size_t capacity) {
    context->status = 0;
    context->request.num_headers = HTTP_HEADERS_LIMIT;
    return (
        buffer_init(&context->request.buffer, capacity) != -1
        && buffer_init(&context->response.buffer, capacity) != -1
    ) ? 0 : -1;
}

int http_context_handle_data(http_context_t* context, const void* data, size_t size) {
    size_t prev_size = context->request.buffer.size;
    if (size && buffer_append(&context->request.buffer, data, size) == -1) {
        return -1;
    }
    if (context->status == 0) {
        int res = phr_parse_request(
            (const char*)context->request.buffer.memory,
            context->request.buffer.size,
            &context->request.method,
            &context->request.method_len,
            &context->request.path,
            &context->request.path_len,
            &context->request.minor_version,
            &context->request.headers[0],
            &context->request.num_headers,
            prev_size
        );
        if (res == -1) {
            context->status = -1;
            return -1;
        } else if (res > 0) {
            context->status = res;
        }
    }
    return 0;
}

void http_context_clear(http_context_t* context) {
    buffer_clear(&context->request.buffer);
    buffer_clear(&context->response.buffer);
}

void http_context_free(http_context_t* context) {
    buffer_free(&context->request.buffer);
    buffer_free(&context->response.buffer);
}

int http_write_data(http_context_t* context, const void* data, size_t size) {
    if (buffer_append(&context->response.buffer, data, size) == -1) {
        context->status = -1;
        return -1;
    }
    return 0;
}

void http_finish(http_context_t* context) {
    if (context->status <= 0) {
        return;
    }
    buffer_consume(&context->request.buffer, (size_t)context->status);
    context->status = 0;
    context->request.num_headers = HTTP_HEADERS_LIMIT;
}
