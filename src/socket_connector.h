#ifndef SOCKET_IO
#define SOCKET_IO

#include "request_reply.h"

request_t* sock_read(const ri_connection_t* connection);
void sock_write(const int sockfd, char* buffer, const int length);

int connect_to_server(char* server_name, const int portno);

#endif
