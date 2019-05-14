#include "../../src/socket/socket_connector.h"
#include <stdlib.h>
#include <string.h>

int sock_read(const int sockfd, char *dest, const int max_length, int flags)
{
	return 0;
}

char* mock_socket_buffer;

int read_from_socket(int fd, char* buffer, int max_size)
{
    if (mock_socket_buffer) {
        strcpy(buffer, mock_socket_buffer);
        return strlen(mock_socket_buffer);
    } else {
        return -1;
    }
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
