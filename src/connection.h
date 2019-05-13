#ifndef CONNECTION_H
#define CONNECTION_H

#include "route_instance.h"

#include <netinet/in.h>
#include "socket/socket_connector.h"

typedef struct {
    int fd;
    int total_messages;
    long total_bytes;
    int last_result;
    int error;
    ri_route_t *route;
} ri_connection_t;

ri_connection_t *wait_4_connection_request(ri_route_t *route);

/*!
  \brief close the file descriptor associated to the described connection, and free the memory allocated to the descriptor
 */
void close_connection(ri_connection_t *conn);

void *receive_and_process_data_from_client(void *params);

void close_in_connector(ri_in_connector_t *conn);

int *create_all_runtime_thread_data(ri_connection_t *conn, int buffer_size, char *buffer, int data_len);

void *socket_connector(void *param);

void *receive_data(void *params);

#endif
