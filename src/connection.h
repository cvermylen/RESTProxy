#ifndef CONNECTION_H
#define CONNECTION_H

#include "route_instance.h"

typedef struct {
        int fd;
	int total_messages;
	long total_bytes;
	int last_result;
	int error;
        ri_route_t* route;
} ri_connection_t;

int* create_all_runtime_thread_data(ri_connection_t *conn, int buffer_size, char *buffer, int data_len);

void* socket_connector(void* param);

void *receive_data(void *params);

void close_connections(ri_route_t* route);
#endif
