#include "socket_connector.h"
#include "accept_connections.h"
#include "socket_io.h"
#include "shared_buffers.h"
#include "routing_handler.h"
#include "route_instance.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

ri_thread_data_t* create_runtime_thread_data(ri_out_connector_t *conn, int buffer_size, char *buffer, int data_len)
{
printf("Create ri_thread_data\n");
	ri_thread_data_t* res = (ri_thread_data_t*)malloc(sizeof(ri_thread_data_t));
	res->buffer_size = buffer_size;
	res->buffer = buffer;
	res->data_len = data_len;
	res->type = conn->type;
	switch(conn->type){
	case TYPE_SOCKET:
		res->content.sock = conn->content.sock;
		break;
	case TYPE_FILE:
		res->content.file = conn->content.file;
		break;
	case TYPE_WORKFLOW:
		res->content.workflow = conn->content.workflow;
		break;
	}
	return res;
}

int create_conn_runtime_thread_data(ri_out_connector_t *conn, int buffer_size, char *buffer, int data_len)
{
	int i;
	for(i = 0; i < MAX_ASYNC_CONCUR && conn->data == NULL; i++);
	if(i < MAX_ASYNC_CONCUR){
		conn->data[i] = create_runtime_thread_data(conn, buffer_size, buffer, data_len);
	}else{
		i = -1;
	}
	return i;
}

void release_conn_runtime_thread_data(ri_thread_data_t* d)
{
printf("Release conn_runtime_thread_data\n");
	free(d);
}

int* create_all_runtime_thread_data(ri_route_t *route, int buffer_size, char *buffer, int data_len)
{
	int *pool = (int *)malloc(sizeof(int) * route->out_connections);
	for(int i=0; i < route->out_connections; i++){
		pool[i] = create_conn_runtime_thread_data(route->out_connectors[i], buffer_size, buffer, data_len);
	}
	return pool;
}

void release_runtime_sock_connector(ri_sock_connector_t *conn)
{
	free(conn);
}

ri_file_connector_t *create_runtime_file_connector(rt_file_connector_t *conn)
{
	ri_file_connector_t * res = (ri_file_connector_t*)malloc(sizeof(ri_file_connector_t));
	res->filename = conn->filename;
	res->file = conn->file;
	res->output_callback = conn->output_callback;
	return res;
}

void release_runtime_file_connector(ri_file_connector_t *conn)
{
	free(conn);
}

ri_out_connector_t *create_runtime_out_connector(rt_out_connector_t *conn)
{
	ri_out_connector_t *res = (ri_out_connector_t*)malloc(sizeof(ri_out_connector_t));
	res->type = conn->type;
	res->flow = conn->flow;
	res->data = (ri_thread_data_t**)malloc(sizeof(ri_thread_data_t*) * MAX_ASYNC_CONCUR);
	memset(res->data, '\0', (sizeof(ri_thread_data_t*) * MAX_ASYNC_CONCUR));
	switch(res->type){
	case TYPE_SOCKET:
		res->content.sock = create_runtime_sock_connector(&conn->content.sock);
		break;
	case TYPE_FILE:
		res->content.file = create_runtime_file_connector(&conn->content.file);
		break;
	case TYPE_WORKFLOW:
		printf("Not implemented yet\n");
		exit(0);
	}
	return res;
}

void release_runtime_out_connector(ri_out_connector_t *conn)
{
	free(conn->data);
	switch(conn->type){
	case TYPE_SOCKET:
		release_runtime_sock_connector(conn->content.sock);
		break;
	case TYPE_FILE:
		release_runtime_file_connector(conn->content.file);
		break;
	case TYPE_WORKFLOW:
		printf("Not implemented yet\n");
		exit(0);
	}
	free(conn);
}

ri_in_connector_t *create_runtime_in_connector(rt_in_connector_t *conn)
{
	ri_in_connector_t *res = (ri_in_connector_t*)malloc(sizeof(ri_in_connector_t));
	res->type = conn->type;
	switch(res->type){
	case TYPE_SOCKET:
		res->content.sock = create_runtime_sock_connector(&conn->content.sock);
		break;
	case TYPE_FILE:
		res->content.file = create_runtime_file_connector(&conn->content.file);
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

ri_route_t *create_runtime_route(rt_route_t *route)
{
	ri_route_t *res = (ri_route_t*)malloc(sizeof(ri_route_t));
	res->out_connections = route->out_connections;
	res->forward_mode = route->forward_mode;
	res->in_connector = create_runtime_in_connector(route->in_connector);
	res->out_connectors = (ri_out_connector_t**)malloc(sizeof(ri_out_connector_t*) * res->out_connections);
	for(int i=0; i < res->out_connections; i++){
		res->out_connectors[i] = create_runtime_out_connector(route->out_connectors[i]);
	}
	return res;
}

void release_runtime_route(ri_route_t *route)
{
	release_runtime_in_connector(route->in_connector);
	for(int i=0; i < route->out_connections; i++){
		release_runtime_out_connector(route->out_connectors[i]);
	}
	free(route);
}
