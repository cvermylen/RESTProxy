#include "../../src/socket/socket_connector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <str_stack.h>

#define MAX_FILE_DESCRIPTOR 5

int bind_port(const int portno) {
    return 0;
}

int sock_read(const int sockfd, char *dest, const int max_length, int flags)
{
	return 0;
}

stack_head_t* mock_socket_receive_buffer[MAX_FILE_DESCRIPTOR];

/*!
 * \private
 * @param fd
 */
void mock_socket_connect_stack_to_fd (int fd)
{
    if (fd > MAX_FILE_DESCRIPTOR) {
        printf("Maximum number of file descriptors set to: %d, defined in mock_socket_connector.c, 'MAX_FILE_DESCRIPTOR'", MAX_FILE_DESCRIPTOR);
        exit(0);
    }
    mock_socket_receive_buffer[fd] = stack_init();
}

/*!
 * \private
 * @param fd
 * @param buffer
 */
void mock_socket_push_buffer(int fd, char* buffer)
{
    str_stack_push(mock_socket_receive_buffer[fd], buffer);
}

/*! Tested
 *
 * @param fd
 * @param buffer
 * @param max_size
 * @return
 */
int read_from_socket(int fd, char* buffer, int max_size)
{
    if (mock_socket_receive_buffer && mock_socket_receive_buffer[fd] && mock_socket_receive_buffer[fd]->num_elems > 0) {
        char* tmp_str = str_stack_pop(mock_socket_receive_buffer[fd]);
        printf("read_from_socket\n");
        strcpy(buffer, tmp_str);
        int result = strlen(tmp_str);
        free(tmp_str);
        return result;
    } else {
        return -1;
    }
}

//TODO: unit test
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
