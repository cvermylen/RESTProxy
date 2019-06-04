#ifndef SOCKET_IO
#define SOCKET_IO

#include <netinet/in.h>

#define BINDING_ERROR	-20000

typedef struct ri_sock_connector ri_sock_connector_t;
typedef struct ri_sock_connector {
    int port;
    int mode;
    int fd;
    struct sockaddr cli_addr;
    socklen_t sockaddr_size;
    char* server_name;
    void *(*consumer_callback)(void * sock_params);  //REFACTOR, remove after the following 3 are implemented

    int (*open_connection) (ri_sock_connector_t* conn_params);
    int (*feed_data) (ri_sock_connector_t* conn_params, char* dest_buffer, int max_buffer_size);
    int (*close_connection) (ri_sock_connector_t* conn_params);
};

/*! Callback method used to open the connection. Uses the connection parameters in the struct to bind the
 * port, and get a file descriptor.
 * @param connection_params
 */
void open_socket_connector (ri_sock_connector_t* connection_params);

int bind_port(const int portno);

int create_input_socket_connector(const int port);

ri_sock_connector_t *create_runtime_sock_connector(const int port, void *socket_connector(void *param));

void sock_write(const int sockfd, char* buffer, const int length);

int read_from_socket(int fd, char* buffer, int max_size);

int socket_connect(char* server_name, const int portno);

void open_server_socket_connector (ri_sock_connector_t* connection_params);

void release_runtime_sock_connector(ri_sock_connector_t *conn);

#endif
