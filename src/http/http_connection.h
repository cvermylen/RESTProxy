#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H

#include "../route.h"

//#include <netinet/in.h>
//#include "../socket/socket_connector.h"
//#include "../route_instance.h"
//#include "../request_reply.h"
#include <stack.h>

typedef struct {
    int fd;
    int (*feeder)(int fd, char* buffer, int buffer_size);
    int total_messages;
    long total_bytes;
    int last_result;
    int error;
    stack_head_t* requestReplies;
    route_t *route;
} ri_connection_t;

ri_connection_t* new_http_connection (route_t* route);

int open_connection(ri_connection_t *conn);

void* run_session (ri_connection_t* conn);

/*!
  \brief close the file descriptor associated to the described connection, and free the memory allocated to the descriptor
 */
void close_connection (ri_connection_t *conn);

void *receive_and_process_data_from_client(void *params);

int *create_all_runtime_thread_data(ri_connection_t *conn, int buffer_size, char *buffer, int data_len);

void *socket_connector(void *param);

void *receive_data(void *params);

void release_runtime_route(ri_connection_t* conn);

#endif
