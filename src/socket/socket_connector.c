#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <stdio.h>
#include "socket_connector.h"
#include "../frame_buffers/shared_buffers.h"
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

int create_input_socket_connector(const int port)
{
    printf("create_input_socket_connector\n");
    return bind_port(port);
}

void release_runtime_sock_connector(ri_sock_connector_t *conn)
{
    free(conn);
}

ri_sock_connector_t *create_runtime_sock_connector(const int port, void *socket_connector(void *param))
{
    printf("create_runtime_sock_connector\n");
    ri_sock_connector_t *res = (ri_sock_connector_t*)malloc(sizeof(ri_sock_connector_t));
    res->port = port;
    res->mode = MODE_TCP;
    res->fd = create_input_socket_connector(port);
    res->consumer_callback = socket_connector;
    return res;
}

int read_from_socket(int fd, char* buffer, int max_size)
{
printf("#####read_from_socket\n");
	int n = wrap_recv(fd, buffer, max_size, 0);
printf("read_from_socket, size:%d\n", n);
	return n;
}

int connect_to_server(char* server_name, const int portno)
{
	int sockfd, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        char buffer[256];
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
	int j = 22;
        if((j = wrap_connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0){
                fprintf(stderr, "ERROR connecting:%s %d", server_name, portno);
		exit(0);
	}
	return sockfd;
}

void sock_write(const int sockfd, char* buffer, const int length)
{
printf("SOCKET send: %d, %s length:%d\n", sockfd, buffer, length);
    wrap_send(sockfd, buffer, length, 0);
}
