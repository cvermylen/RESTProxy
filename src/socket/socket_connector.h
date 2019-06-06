#ifndef SOCKET_IO
#define SOCKET_IO

#include <netinet/in.h>
#include "../route_instance.h"

#define BINDING_ERROR	-20000

typedef struct ri_sock_connector ri_sock_connector_t;
typedef struct ri_sock_connector {
    int port;
    int mode;
    int fd;
    struct sockaddr cli_addr;
    socklen_t sockaddr_size;
    char* server_name;
};

/*! Callback method used to open the connection. Uses the connection parameters in the struct to bind the
 * port, and get a file descriptor.
 * @param connection_params
 */
int open_socket_connector (ri_sock_connector_t* connection_params);

int bind_port(int portno);

int create_input_socket_connector(int port);

void create_runtime_sock_connector(ri_in_connector_t *res, int port_no);

void sock_write(int sockfd, char* buffer, int length);

int read_from_socket(int fd, char* buffer, int max_size);

int socket_connect(char* server_name, int portno);

void open_server_socket_connector (ri_sock_connector_t* connection_params);

void close_socket (ri_sock_connector_t* connection_params);

void release_runtime_sock_connector(ri_sock_connector_t *conn);

#endif
