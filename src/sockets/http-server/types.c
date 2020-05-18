#include "types.h"

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

static int eo_parse_addr_limited(const char* s, size_t n, in_port_t port, eo_addr_t* dest) {
    if (n > INET6_ADDRSTRLEN) {
        n = INET6_ADDRSTRLEN;
    }
    if (n >= 2 && *s == '[' && s[n - 1] == ']') {
        s += 1;
        n -= 2;
    }
    char buf[INET6_ADDRSTRLEN + 1] = {0};
    memcpy(buf, s, n);
    if (inet_pton(AF_INET6, buf, &dest->v6.sin6_addr) > 0) {
        dest->v6.sin6_port = (in_port_t)htons(port);
        dest->v6.sin6_family = AF_INET6;
        return 0;
    }
    if (inet_pton(AF_INET, buf, &dest->v4.sin_addr) > 0) {
        dest->v4.sin_port = (in_port_t)htons(port);
        dest->v4.sin_family = AF_INET;
        return 0;
    }
    return -1;
}

int eo_parse_addr(const char* s, in_port_t port, eo_addr_t* dest) {
    return eo_parse_addr_limited(s, strlen(s), port, dest);
}
int eo_parse_addr_port(const char* s, eo_addr_t* dest) {
    size_t n = strlen(s);

    for (size_t i = n; i-- > 0;) {
        if (s[i] == ':') {
            int value = atoi(&s[i + 1]);
            if (value <= 0 || value > SHRT_MAX) {
                return -1;
            }
            in_port_t port = (in_port_t)value;
            return eo_parse_addr_limited(s, i, port, dest);
        }
    }
    return -1;
}

int eo_is_v4_addr(const eo_addr_t* addr) {
    return addr->base.sa_family == AF_INET;
}

int eo_is_v6_addr(const eo_addr_t* addr) {
    return addr->base.sa_family == AF_INET6;
}

int eo_addr_to_str(const eo_addr_t* addr, eo_addr_str_t* dest) {
    char buf[INET6_ADDRSTRLEN + 1] = {0};
    in_port_t port;
    if (eo_is_v4_addr(addr)) {
        inet_ntop(AF_INET, &addr->v4.sin_addr.s_addr, buf, INET_ADDRSTRLEN);
        port = (in_port_t)ntohs(addr->v4.sin_port);
    } else if (eo_is_v6_addr(addr)) {
        inet_ntop(AF_INET6, &addr->v6.sin6_addr.s6_addr, buf, INET6_ADDRSTRLEN);
        port = (in_port_t)ntohs(addr->v6.sin6_port);
    } else {
        return -1;
    }
    sprintf(*dest, "%s:%"PRIu16, buf, port);
    return 0;
}

size_t eo_addr_size(const eo_addr_t* addr) {
    return (
        eo_is_v4_addr(addr) ? sizeof(struct sockaddr_in)
        : eo_is_v6_addr(addr) ? sizeof(struct sockaddr_in6)
        : 0
    );
}
