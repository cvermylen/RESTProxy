#include "socket/socket_connector.h"
#include "frame_buffers/shared_buffers.h"
#include "route_def.h"
#include "route_instance.h"
#include "file_connector.h"
#include "request_reply.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

int program_should_continue = 1;

void *socket_connector(void *param) {
    ri_route_t *route = (ri_route_t *) param;
//    connections_stack = stack_init();
    printf("socket_connector\n");
    pthread_t thread;
    do {
        ri_connection_t *cli = wait_4_connection_request(route);
        if (cli->fd > 0) {
//            stack_push(connections_stack, cli);
            printf("%d\n", cli->fd);
            int rc = pthread_create(&thread, NULL, receive_and_process_data_from_client, (void *) cli);
        }
    } while (program_should_continue);
    printf("END SOCKET CONNECTOR\n");
    return NULL;
}

void close_connections(ri_route_t *route) {
    printf("Close connection\n");
    close_in_connector(route->in_connector);
    program_should_continue = 0;
}

ri_out_connector_t* create_runtime_out_sock_connector(const int flow, const char* hostname, const int port)
{
	ri_sock_connector_t* res = (ri_sock_connector_t*)malloc(sizeof(ri_sock_connector_t));
	res->port = port;
	res->server_name = (char*)malloc(sizeof(char) * strlen(hostname));
	strcpy(res->server_name, hostname);
	res->consumer_callback = sync_request_reply_to_server;

	ri_out_connector_t* conn = (ri_out_connector_t*)malloc(sizeof(ri_out_connector_t));
	conn->type = TYPE_SOCKET;
	conn->flow = flow;
	conn->content.sock = res;
	if(flow == FLOW_BIDIRECTIONAL) {
		conn->response_callback = reply_to_client;
	}
	return conn;
}

ri_out_connector_t *create_runtime_out_file_connector(const int flow, const char* filename)
{
printf("create_runtime_file_connector\n");
	ri_file_connector_t * res = (ri_file_connector_t*)malloc(sizeof(ri_file_connector_t));
	res->filename = (char*)malloc((strlen(filename) + 1) * sizeof(char));
	strcpy(res->filename, filename);
	res->file = fopen(res->filename, "w");
	res->output_callback = file_writer;

	ri_out_connector_t* conn = (ri_out_connector_t*)malloc(sizeof(ri_out_connector_t));
	conn->type = TYPE_FILE;
	conn->flow = flow;
	conn->content.file = res;
	if(flow == FLOW_BIDIRECTIONAL){
		conn->response_callback = reply_to_client;
	}
	return conn;
}

void release_runtime_out_connector(ri_out_connector_t *conn)
{
	switch(conn->type){
	case TYPE_SOCKET:
		release_runtime_sock_connector(conn->content.sock);
		break;
	case TYPE_FILE:
		release_runtime_file_connector(conn->content.file);
		break;
	}
	free(conn);
}

ri_in_connector_t *create_runtime_in_connector(const int type, const int port)
{
printf("create_runtime_in_connector\n");
	ri_in_connector_t *res = (ri_in_connector_t*)malloc(sizeof(ri_in_connector_t));
	res->type = type;
	switch(type){
	case TYPE_SOCKET:
		res->content.sock = create_runtime_sock_connector(port, socket_connector);
		break;
	case TYPE_FILE:
		// res->content.file = create_runtime_file_connector(&(conn->content.file));
		break;
	}
	return res;
}

void release_runtime_in_connector(ri_in_connector_t *conn)
{
	switch(conn->type){
	case TYPE_SOCKET:
		release_runtime_sock_connector(conn->content.sock);
		break;
	case TYPE_FILE:
		release_runtime_file_connector(conn->content.file);
		break;
	}
	free(conn);
}

ri_route_t *create_runtime_route(const int port, const int mode, const int num_connectors)
{
printf("create_runtime_route\n");
	ri_route_t *res = (ri_route_t*)malloc(sizeof(ri_route_t));
	res->out_connections = num_connectors;
	res->forward_mode = mode;
	res->in_connector = create_runtime_in_connector(TYPE_SOCKET, port);
	res->out_connectors = (ri_out_connector_t**)malloc(sizeof(ri_out_connector_t*) * num_connectors);
	return res;
}

void add_out_file_connector(ri_route_t* route, const int index, const char* filename, const int flow)
{
	route->out_connectors[index] = create_runtime_out_file_connector(flow, filename);
}

void add_out_sock_connector(ri_route_t* route, const int index, const char* hostname, const int port, const int flow)
{
	route->out_connectors[index] = create_runtime_out_sock_connector(flow, hostname, port);
}

void release_runtime_route(ri_route_t *route)
{
	close_connections(route);
	release_runtime_in_connector(route->in_connector);
	for(int i=0; i < route->out_connections; i++){
		release_runtime_out_connector(route->out_connectors[i]);
	}
	free(route);
}

void start_in_listener(ri_route_t* route)
{
	pthread_t pthread;
	int i = pthread_create(&pthread, NULL, route->in_connector->content.sock->consumer_callback, (void*)route);
	pthread_join(pthread, NULL);
}
