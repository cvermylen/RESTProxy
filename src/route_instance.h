#ifndef ROUTE_INSTANCE_H
#define ROUTE_INSTANCE_H
#include <pthread.h>
#include <stdio.h>
#include <netinet/in.h>

typedef struct {
        int port;
        int mode;
        int fd;
	struct sockaddr cli_addr;
        socklen_t sockaddr_size;
	char* server_name;
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
	char* msg;
} ri_response_t;

typedef struct {
        int type;
        int flow;
        union {
                ri_sock_connector_t *sock;
                ri_file_connector_t *file;
                ri_workflow_connector_t *workflow;
        } content;
	void (*response_callback)(void* ri_response);
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

ri_sock_connector_t *create_runtime_sock_connector(const int port);

void add_out_file_connector(ri_route_t* route, const int index, const char* filename, const int flow);

void add_out_sock_connector(ri_route_t* route, const int index, const char* hostname, const int port, const int flow);
ri_out_connector_t *create_runtime_out_file_connector(const int flow, const char* filename);
/*!
  \brief free all memory allocated to the route instance, except the thread data (threads may still be running)
 */
void release_runtime_route(ri_route_t *route);

void start_in_listener(ri_route_t* route);

#endif
