#ifndef SOCKET_IO
#define SOCKET_IO

#include <netinet/in.h>

#define BINDING_ERROR	-20000

typedef struct {
    int port;
    int mode;
    int fd;
    struct sockaddr cli_addr;
    socklen_t sockaddr_size;
    char* server_name;
    void *(*consumer_callback)(void * sock_params);
} ri_sock_connector_t;

int bind_port(const int portno);

int create_input_socket_connector(const int port);

ri_sock_connector_t *create_runtime_sock_connector(const int port, void *socket_connector(void *param));

void sock_write(const int sockfd, char* buffer, const int length);

int read_from_socket(int fd, char* buffer, int max_size);

int connect_to_server(char* server_name, const int portno);

void release_runtime_sock_connector(ri_sock_connector_t *conn);

#endif
