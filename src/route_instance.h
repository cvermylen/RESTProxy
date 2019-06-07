#ifndef ROUTE_INSTANCE_H
#define ROUTE_INSTANCE_H
#include <pthread.h>

#include "file_connector.h"


typedef struct {
        char *filename;
        FILE *file;
} ri_workflow_connector_t;

typedef struct {
    int type;
	pthread_t pthread;  // ?????????????
	void* connection_params;
    int (*open_connection) (void* conn_params);
    int (*feed_data) (void* conn_params, char* dest_buffer, int max_buffer_size);
    int (*send_data) (void* conn_params, char* dest_buffer, int max_buffer_size);
    int (*close_connection) (void* conn_params);
} ri_in_connector_t;

typedef struct {
    int type;
    int flow;
    void* connection_params;
    int (*open_connection) (void* conn_params);
    int (*send_data) (void* conn_params, char* dest_buffer, int max_buffer_size);
    int (*receive_data) (void* conn_params, char* dest_buffer, int max_buffer_size);
    int (*close_connection) (void* conn_params);
} ri_out_connector_t;

typedef struct {
	int out_connections;
	ri_in_connector_t *in_connector;
	ri_out_connector_t **out_connectors;
	int forward_mode;
} ri_route_t;

/*!
  \brief Finds an available container to hold the data that the connector instance thread will send out
 */
int create_conn_runtime_thread_data(int fd, ri_in_connector_t* in_conn, ri_out_connector_t *conn, int buffer_size, char *buffer, int data_len);

ri_route_t *create_runtime_route(const int port, const int mode, const int num_connectors);

void add_out_file_connector(ri_route_t* route, const int index, const char* filename, const int flow);

void add_out_sock_connector(ri_route_t* route, const int index, const char* hostname, const int port, const int flow);
ri_out_connector_t *create_runtime_out_file_connector(const int flow, const char* filename);
/*!
  \brief free all memory allocated to the route instance, except the thread data (threads may still be running)
 */
void release_runtime_route(ri_route_t *route);

void start_route(ri_route_t *route);

#endif
