#ifndef ROUTING_HANDLER_H
#define ROUTING_HANDLER_H

#include "routing_params.h"

#define MAX_ASYNC_CONCUR	1

typedef struct {
        int port;
        int mode;
        int fd;
        void *(*consumer_callback)(void * sock_params);
} rt_sock_connector_t;

typedef struct {
        char *filename;
        FILE *file;
        void (*output_callback)(void *parms);
} rt_file_connector_t;

typedef struct {
        char *filename;
        FILE *file;
} rt_workflow_connector_t;

typedef struct {
        int type;
        union {
                rt_sock_connector_t sock;
                rt_file_connector_t file;
        } content;
} rt_in_connector_t;

typedef struct {
        int type;
        int flow;
        union {
                rt_sock_connector_t sock;
                rt_file_connector_t file;
                rt_workflow_connector_t workflow;
        } content;
} rt_out_connector_t;

typedef struct {
	int source_fd;
	int num_callbacks;
	rt_out_connector_t **out_connectors;
	int dest_size;
	int forward_mode;
} sock_thread_params_t;

typedef struct {
        int out_connections;
        rt_in_connector_t *in_connector;
        rt_out_connector_t **out_connectors;
        int forward_mode;
} rt_route_t;

void init_all_routes(route_t** routes, const int max_routes);

#endif
