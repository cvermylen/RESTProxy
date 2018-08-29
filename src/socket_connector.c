#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <stdio.h>
#include "socket_connector.h"
#include "shared_buffers.h"

int read_from_socket(int fd, char* buffer, int max_size)
{
printf("#####read_from_socket\n");
	int n = recv(fd, buffer, max_size, 0);
printf("read_from_socket, size:%d\n", n);
	return n;
}

int connect_to_server(char* server_name, const int portno)
{
	int sockfd, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
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
	int j = 22;
        if((j = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0){
                fprintf(stderr, "ERROR connecting:%s %d", server_name, portno);
		exit(0);
	}
	return sockfd;
}

void sock_write(const int sockfd, char* buffer, const int length)
{
printf("SOCKET send: %d, %s length:%d\n", sockfd, buffer, length);
	send(sockfd, buffer, length, 0);
}
