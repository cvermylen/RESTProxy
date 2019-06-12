#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H

#include "../route.h"
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

void *socket_connector(void *param);

void release_runtime_route(ri_connection_t* conn);

#endif
