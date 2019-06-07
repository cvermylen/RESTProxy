#include "socket/socket_connector.h"
#include "buffers/shared_buffers.h"
#include "route_def.h"
#include "route_instance.h"
#include "file_connector.h"
#include "request_reply.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

ri_out_connector_t* create_runtime_out_sock_connector(const int flow, const char* hostname, const int port)
{
	ri_sock_connector_t* res = (ri_sock_connector_t*)malloc(sizeof(ri_sock_connector_t));
	res->port = port;
	res->server_name = (char*)malloc(sizeof(char) * strlen(hostname));
	strcpy(res->server_name, hostname);

	ri_out_connector_t* conn = (ri_out_connector_t*)malloc(sizeof(ri_out_connector_t));
	conn->type = TYPE_SOCKET;
	conn->flow = flow;
	conn->connection_params = res;
	conn->open_connection = (int (*) (void*))open_server_socket_connector;
	conn->send_data = (int (*) (void*, char*, int))sock_write;
    conn->receive_data = (int (*) (void*, char*, int))read_from_socket;
    conn->close_connection = (int (*) (void*))close_socket;
	return conn;
}

ri_out_connector_t *create_runtime_out_file_connector(const int flow, const char* filename)
{
printf("create_runtime_file_connector\n");
	ri_file_connector_t * res = (ri_file_connector_t*)malloc(sizeof(ri_file_connector_t));
	res->filename = (char*)malloc((strlen(filename) + 1) * sizeof(char));
	strcpy(res->filename, filename);

	ri_out_connector_t* conn = (ri_out_connector_t*)malloc(sizeof(ri_out_connector_t));
    conn->connection_params = res;
	conn->type = TYPE_FILE;
	conn->flow = flow;
	conn->open_connection = (int (*) (void*))open_file;
	conn->send_data = (int (*) (void*, char*, int))file_writer;
	conn->receive_data = (int (*) (void*, char*, int))file_reader;
	conn->close_connection = (int (*) (void*))close_file;
	return conn;
}

void release_runtime_out_connector(ri_out_connector_t *conn)
{
	free(conn);
}

ri_in_connector_t *create_runtime_in_connector(const int type, const int port)
{
printf("create_runtime_in_connector\n");
	ri_in_connector_t *res = (ri_in_connector_t*)malloc(sizeof(ri_in_connector_t));
	res->type = type;
	switch(type){
	case TYPE_SOCKET:
	    create_runtime_sock_connector (res, port);
		res->feed_data = (int (*) (void*, char*, int))read_from_socket;
		res->send_data = (int (*) (void*, char*, int))sock_write;
		break;
	case TYPE_FILE:
		// res->content.file = create_runtime_file_connector(&(conn->content.file));
		break;
	}
	return res;
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

void free_route (ri_route_t* route)
{
    release_runtime_in_connector (route->in_connector);
    for (int i=0; i < route->out_connections; i++) {
        release_runtime_out_connector(route->out_connectors[i]);
    }
}

void release_runtime_route(ri_connection_t* conn)
{
    stack_node_t* rr = NULL;
    while ((rr = stack_pop (conn->requestReplies)) != NULL ) {
        close_in_and_out_connections (rr->elem);
    }
    free_request_replies (rr->elem);
	free(conn->requestReplies);

	free_route (conn->route);
	free(conn);
}

ri_connection_t* start_route(ri_route_t *route)
{
	pthread_t pthread;
	ri_connection_t* conn = new_http_connection(route);
	int i = pthread_create (&pthread, NULL, (void* (*) (void*))run_session, conn);
	pthread_join(pthread, NULL);
	return conn;
}
