#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "mock_socket.h"

int sock_rval;
int function_called;

//These should be freed @ end of the test
static int next_ptr = 0;
struct sockaddr* cli_addr[10];
socklen_t* cli_addr_len[10];

int wrap_bind(int socket, const struct sockaddr *address, socklen_t address_len){
	function_called |= MOCK_SOCKET_BIND;
	return 0;
}

int wrap_accept(int socket, struct sockaddr *address, socklen_t *address_len)
{
	cli_addr[next_ptr] = (struct sockaddr*) malloc(sizeof(struct sockaddr));
	cli_addr_len[next_ptr] = (socklen_t*) malloc(sizeof(socklen_t));
	next_ptr += 1;
	function_called |= MOCK_SOCKET_ACCEPT;
	return (socket >0)?1:-1;
}

int wrap_socket(int domain, int type, int protocol)
{
	function_called |= MOCK_SOCKET_SOCKET;
	return sock_rval;
}

int wrap_read(int sockfd, char *buffer, int size)
{
	function_called |= MOCK_SOCKET_READ;
	return sock_rval;
}

void free_mock_sockets()
{
	int i;
	for(i = 0; i < next_ptr; i++){
		free(cli_addr[i]);
		free(cli_addr_len[i]);
	}
	next_ptr = 0;
}

int wrap_setsockopt (int socket, int level, int option_name, const void *option_value, socklen_t option_len)
{
	function_called |= MOCK_SOCKET_OPT;
	return 0;
}

int wrap_listen(int fd, int q)
{
	function_called |= MOCK_SOCKET_LISTEN;
	return 0;
}

int wrap_connect(int sockfd, const struct sockaddr* serv_addr, socklen_t option_len)
{
    return 0;
}

int wrap_send(int sockfd, const void *buf, size_t len, int flags)
{
    return 0;
}
int wrap_recv(int sockfd, char *buffer, int size, int flags)
{
    return 0;
}
