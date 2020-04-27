```c
struct addrinfo
{
    int              ai_flags;     // флаги для использования в hints
    int              ai_family;    // для TCP/IP v4 здесь AF_INET, используется в hints
    int              ai_socktype;  // для TCP здесь SOCK_STREAM, используется в hints
    int              ai_protocol;  // уточнение протокола, обычно 0
    socklen_t        ai_addrlen;   // длина адреса для передачи в параметре addrlen в connect или bind
    struct sockaddr *ai_addr;      // адрес для передачи в connect или bind
    char            *ai_canonname;
    struct addrinfo *ai_next;      // указатель на следующий элемент
};
```