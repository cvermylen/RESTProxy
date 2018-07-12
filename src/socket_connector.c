#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <stdio.h>
#include "socket_connector.h"
#include "shared_buffers.h"

request_t* sock_read(const ri_connection_t* connection)
{
	int buffer_no = alloc_buffer();
	char* buffer = get_buffer(buffer_no);
	request_t* request = create_request(connection, buffer_no, buffer);
printf("SOCKET read:%d\n", request->in_response.sock_fd);
	int n = recv(request->in_response.sock_fd, request->buffer, TX_BUFFER_SIZE, 0);
	request->data_len = n;
printf("Read:%d\n", n);
	return request;
}

void sync_request_reply_to_server(reply_t* reply)
{
printf("sync_request_reply_to_server\n");
	int socket = connect_to_server(reply->content.sock->server_name, reply->content.sock->port);
	reply->content.sock->fd = socket;
}
	
int connect_to_server(char* server_name, const int portno)
{
	int sockfd, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
printf("connect_to_server: %s %d\n", server_name, portno);
        char buffer[256];
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
        if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
                fprintf(stderr, "ERROR connecting:%s %d", server_name, portno);
		exit(0);
printf("connect_to_server: %d\n", portno);
	return sockfd;
}

void sock_write(const int sockfd, char* buffer, const int length)
{
printf("SOCKET send: %d, %s length:%d\n", sockfd, buffer, length);
	send(sockfd, buffer, length, 0);
}
