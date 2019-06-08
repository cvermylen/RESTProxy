#ifndef SOCKET_IO
#define SOCKET_IO

#include <netinet/in.h>
//#include "../route_instance.h"

#define BINDING_ERROR	-20000

typedef struct ri_sock_connector {
    int port;
    int mode;
    int fd;
    struct sockaddr cli_addr;
    socklen_t sockaddr_size;
    char* server_name;
} ri_sock_connector_t;

/*! Callback method used to open the connection. Uses the connection parameters in the struct to bind the
 * port, and get a file descriptor.
 * @param connection_params
 */
int open_socket_connector (ri_sock_connector_t* connection_params);

int bind_port(int portno);

int create_input_socket_connector(int port);

ri_sock_connector_t* create_runtime_sock_connector(int port_no);

int sock_write(int sockfd, char* buffer, int length);

int read_from_socket(int fd, char* buffer, int max_size);

int socket_connect(char* server_name, int portno);

int open_server_socket_connector (ri_sock_connector_t* connection_params);

int close_socket (ri_sock_connector_t* connection_params);

#endif
