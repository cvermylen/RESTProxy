#ifndef ROUTE_H
#define ROUTE_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

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
} in_connector_t;

typedef struct {
    int type;
    int flow;
    void* connection_params;
    int (*open_connection) (void* conn_params);
    int (*send_data) (void* conn_params, char* dest_buffer, int max_buffer_size);
    int (*receive_data) (void* conn_params, char* dest_buffer, int max_buffer_size);
    int (*close_connection) (void* conn_params);
} out_connector_t;

typedef struct {
    int out_connections;
    in_connector_t *in_connector;
    out_connector_t **out_connectors;
    int forward_mode;

} route_t;


route_t *create_route(int port, int mode, int num_connectors);


void add_out_file_connector(route_t* route, int index, const char* filename, int flow);

void add_out_sock_connector(route_t* route, int index, const char* hostname, int port, int flow);
out_connector_t *create_runtime_out_file_connector(int flow, const char* filename);

void free_route (route_t* route);
#endif //ROUTE_H
