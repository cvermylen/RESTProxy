#ifndef ROUTE_INSTANCE_H
#define ROUTE_INSTANCE_H
#include <pthread.h>
#include <stdio.h>
#include <netinet/in.h>
#include "routing_handler.h"

typedef struct {
        int port;
        int mode;
        int fd;
	struct sockaddr cli_addr;
        socklen_t sockaddr_size;
        void *(*consumer_callback)(void * sock_params);
} ri_sock_connector_t;

typedef struct {
        char *filename;
        FILE *file;
        void (*output_callback)(void *parms);
} ri_file_connector_t;

typedef struct {
        char *filename;
        FILE *file;
} ri_workflow_connector_t;

typedef struct {
        int type;
	pthread_t pthread;
        union {
                ri_sock_connector_t *sock;
                ri_file_connector_t *file;
        } content;
} ri_in_connector_t;

typedef struct {
	int buffer_size;
	char *buffer;
	int data_len;
	pthread_t pthread;
	int type;
	union {
                ri_sock_connector_t *sock;
                ri_file_connector_t *file;
                ri_workflow_connector_t *workflow;
        } content;
} ri_thread_data_t;

typedef struct {
        int type;
        int flow;
	ri_thread_data_t **data;
        union {
                ri_sock_connector_t *sock;
                ri_file_connector_t *file;
                ri_workflow_connector_t *workflow;
        } content;
} ri_out_connector_t;

typedef struct {
	int out_connections;
	ri_in_connector_t *in_connector;
	ri_out_connector_t **out_connectors;
	int forward_mode;
} ri_route_t;

ri_route_t *create_runtime_route(rt_route_t *route);
int* create_all_runtime_thread_data(ri_route_t *route, int buffer_size, char *buffer, int data_len);

/*!
  \brief free all memory allocated to the route instance, except the thread data (threads may still be running)
 */
void release_runtime_route(ri_route_t *route);

void release_conn_runtime_thread_data(ri_thread_data_t* d);
#endif
