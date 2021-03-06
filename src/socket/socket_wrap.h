#ifndef GATEWAY_SOCKET_WRAP_H
#define GATEWAY_SOCKET_WRAP_H

int wrap_bind(int socket, const struct sockaddr *address, socklen_t address_len);

int wrap_accept(int socket, struct sockaddr *address, socklen_t *address_len);

int wrap_socket(int domain, int type, int protocol);

int wrap_recv(int sockfd, char *buffer, int size, int flags);

int wrap_setsockopt (int socket, int level, int option_name, const void *option_value, socklen_t option_len);

int wrap_listen(int fd, int q);

int wrap_connect(int sockfd, const struct sockaddr* serv_addr, socklen_t option_len);

int wrap_send(int sockfd, const void *buf, size_t len, int flags);

#endif //GATEWAY_SOCKET_WRAP_H
