#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>

#include "socket_connector.h"
#include "../route_def.h"
#include "socket_wrap.h"

//TODO Need to be unit tested
int bind_port(const int portno) {
    printf("bind_port %d\n", portno);
    if (portno <= 0 || portno > 65535)
        return -1;

    struct sockaddr_in serv_addr;
    int sockfd;

    int true = 1;
    sockfd = wrap_socket(AF_INET, SOCK_STREAM, 0);
    wrap_setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int));
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (wrap_bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        return BINDING_ERROR;
    wrap_listen(sockfd, 5);
    printf("binded port:%d\n", sockfd);
    return sockfd;
}

int socket_connect(char* server_name, const int portno)
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = wrap_socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        fprintf(stderr, "connect_to_server:ERROR opening socket");
        return -1;
    }
    server = gethostbyname(server_name);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host:%s\n", server_name);
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if((wrap_connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0){
        fprintf(stderr, "ERROR connecting:%s %d", server_name, portno);
        exit(0);
    }
    return sockfd;
}

int create_input_socket_connector(const int port)
{
    printf("create_input_socket_connector\n");
    return bind_port(port);
}

void release_runtime_sock_connector(ri_sock_connector_t *conn)
{
    free(conn);
}

int open_socket_connector (ri_sock_connector_t* connection_params)
{
    connection_params->fd = create_input_socket_connector(connection_params->port);
    connection_params->fd = wrap_accept(connection_params->fd, &(connection_params->cli_addr), &(connection_params->sockaddr_size));
    return connection_params->fd;
}

void open_server_socket_connector (ri_sock_connector_t* connection_params)
{
    connection_params->fd = socket_connect (connection_params->server_name, connection_params->port);
}

void create_runtime_sock_connector (ri_in_connector_t *res, int port_no)
{
    printf("create_runtime_sock_connector\n");
    ri_sock_connector_t *sc = (ri_sock_connector_t*)malloc(sizeof(ri_sock_connector_t));
    sc->port = port_no;
    sc->mode = MODE_TCP;
    res->connection_params = sc;
    res->open_connection = open_socket_connector;
    res->close_connection = close_socket;
}

int read_from_socket(int fd, char* buffer, int max_size)
{
printf("#####read_from_socket\n");
	int n = wrap_recv(fd, buffer, max_size, 0);
printf("read_from_socket, size:%d\n", n);
	return n;
}

void close_socket (ri_sock_connector_t* connection_params)
{
    close (connection_params->fd);
}

void sock_write(const int sockfd, char* buffer, const int length)
{
printf("SOCKET send: %d, %s length:%d\n", sockfd, buffer, length);
    wrap_send(sockfd, buffer, length, 0);
}
