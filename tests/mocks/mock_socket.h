#ifndef MOCK_SOCKET_H
#define MOCK_SOCKET_H

#include <netinet/in.h>

extern int sock_rval;

extern int function_called;
#define MOCK_SOCKET_BIND	1
#define MOCK_SOCKET_ACCEPT	2
#define MOCK_SOCKET_SOCKET	4
#define MOCK_SOCKET_READ	8
#define MOCK_SOCKET_OPT		16
#define MOCK_SOCKET_LISTEN	32

int wrap_socket(int domain, int type, int protocol);

int wrap_setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);

int wrap_bind(int port, const struct sockaddr* sockaddr, socklen_t size);

int wrap_listen(int fd, int q);

int wrap_accept(int socket, struct sockaddr *address, socklen_t *address_len);

int wrap_read(int, char *, int);

void free_mock_sockets();

#endif
