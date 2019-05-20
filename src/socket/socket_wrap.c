#include <sys/socket.h>
#include "socket_wrap.h"

int wrap_bind(int socket, const struct sockaddr *address, socklen_t address_len){
    return bind(socket, address, address_len);
}

int wrap_accept(int socket, struct sockaddr *address, socklen_t *address_len)
{
    return accept(socket, address, address_len);
}

int wrap_socket(int domain, int type, int protocol)
{
    return socket(domain, type, protocol);
}

int wrap_recv(int sockfd, char *buffer, int size, int flags)
{
    return recv(sockfd, buffer, size, flags);
}

int wrap_setsockopt (int socket, int level, int option_name, const void *option_value, socklen_t option_len)
{
    return setsockopt(socket, level, option_name, option_value, option_len);;
}

int wrap_listen(int fd, int q)
{
    return listen(fd, q);
}

int wrap_connect(int sockfd, const struct sockaddr* serv_addr, socklen_t option_len)
{
    return connect(sockfd, serv_addr, option_len);
}

int wrap_send(int sockfd, const void *buf, size_t len, int flags)
{
    return send(sockfd, buf, len, flags);
}