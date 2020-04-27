#pragma once

#include <stddef.h>
#include <arpa/inet.h>

#define EO_ADDR_LEN (INET6_ADDRSTRLEN + 18)

typedef char eo_addr_str_t[EO_ADDR_LEN];

typedef struct {
    const char* data;
    const size_t size;
} eo_string_view_t;

typedef struct {
    union {
        struct sockaddr base;
        struct sockaddr_in v4;
        struct sockaddr_in6 v6;
    };
} eo_addr_t;

// parse ipv4/ipv6 address. returns 0 on success, -1 on error.
// on success dest will be set as sockaddr_in or sockaddr_in6.
int eo_parse_addr(const char* s, in_port_t port, eo_addr_t* dest);

// parse ipv4/ipv6 address and port. returns 0 on success, -1 on error.
// on success dest will be set as sockaddr_in or sockaddr_in6.
int eo_parse_addr_port(const char* s, eo_addr_t*);

// convert sockaddr_in/sockaddr6_in to string. returns 0 on success, -1 on error.
// on success string will be set into dest.
int eo_addr_to_str(const eo_addr_t*, eo_addr_str_t* dest);

// returns 1 if ipv4 addr, else 0.
int eo_is_v4_addr(const eo_addr_t*);

// returns 1 if ipv6 addr, else 0.
int eo_is_v6_addr(const eo_addr_t*);

// returns sizeof(sockaddr6_in) if addr is ipv6.
// returns sizeof(sockaddr_in) if addr is ipv4.
// else returns 0.
size_t eo_addr_size(const eo_addr_t*);
