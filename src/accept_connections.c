#include "route_def.h"
#include "accept_connections.h"
#include "route_instance.h"
#include "connection.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int bind_port(const int portno)
{
printf("bind_port %d\n", portno);
	if(portno <= 0 || portno > 65535)
		return -1;

	struct sockaddr_in serv_addr;
	int sockfd;

	int true = 1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		return BINDING_ERROR;
	listen(sockfd, 5);
printf("binded port:%d\n", sockfd);
	return sockfd;
}

ri_connection_t* wait_4_connection_request(ri_route_t *route)
{
	ri_connection_t* res = (ri_connection_t*)malloc(sizeof(ri_connection_t));
	res->route = route;
	ri_sock_connector_t *conn = route->in_connector->content.sock;
printf("BEFORE ACCEPTING new connection:%d\n", conn->fd);
	res->fd = accept(conn->fd, &(conn->cli_addr), &(conn->sockaddr_size));	
	if(res->fd <0) {printf("EXIT\n");}
printf("AFTER ACCEPTED new connection:%d\n", res->fd);
	return res;
}

void close_connection(ri_connection_t* conn)
{
printf("Close connection:%d\n", conn->fd);
	close(conn->fd);
}
