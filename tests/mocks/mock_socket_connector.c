#include "../../src/socket_connector.h"
#include <stdlib.h>
#include <string.h>

request_t* sock_read(const ri_connection_t* connection)
{
	return NULL;
}

char* mock_socket_buffer;
int read_from_socket(int fd, char* buffer, int max_size)
{
	strcpy(buffer, mock_socket_buffer);
	return strlen(mock_socket_buffer);
}

int split_receive(int socket, char* buffer, int max_size)
{
	return -1;
}

int connect_to_server(char* server_name, const int portno)
{
	return -1;
}

void sock_write(const int sockfd, char* buffer, const int length)
{
}
