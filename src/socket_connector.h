#ifndef SOCKET_IO
#define SOCKET_IO

#include "request_reply.h"

void sock_write(const int sockfd, char* buffer, const int length);

int read_from_socket(int fd, char* buffer, int max_size);

void* sync_request_reply_to_server(reply_t* reply);

int connect_to_server(char* server_name, const int portno);

#endif
