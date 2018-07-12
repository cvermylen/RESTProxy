#include "accept_connections.h"
#include "route_instance.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int bind_port(const int portno)
{
	if(portno < 0 || portno > 65535)
		return -1;

	struct sockaddr_in serv_addr;
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		return BINDING_ERROR;
	listen(sockfd, 5);
	return sockfd;
}

ri_route_t* wait_4_connection_request(rt_route_t *route)
{
	ri_route_t *res = create_runtime_route(route);

	res->in_connector->content.sock->sockaddr_size = sizeof(struct sockaddr_in);
	ri_sock_connector_t *conn = res->in_connector->content.sock;
printf("BEFORE ACCEPTING new connection\n");
	conn->fd = accept(route->in_connector->content.sock.fd, &(conn->cli_addr), &(conn->sockaddr_size));	
printf("AFTER ACCEPTED new connection:%d\n", conn->fd);
	return res;
}

void close_connection(ri_route_t *sp)
{
	close(sp->in_connector->content.sock->fd);
	release_runtime_route(sp);
}
