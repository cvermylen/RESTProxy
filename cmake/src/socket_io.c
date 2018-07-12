#include <sys/socket.h>
#include <strings.h>
#include <stdio.h>
#include "socket_io.h"

int sock_read(const int sockfd, char *dest, const int max_length, int flags)
{
	bzero(dest, max_length);
	int n = recv(sockfd, dest, max_length, flags);
	return n;
}
