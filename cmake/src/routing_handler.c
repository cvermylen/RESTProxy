#include "routing_handler.h"
#include "routing_params.h"
#include "accept_connections.h"
#include "socket_connector.h"
#include "file_connector.h"
#include "workflow_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int create_input_socket_connector(const sock_connector_t sock_p)
{
	return bind_port(sock_p.port);
}

static void start_socket_listener(const rt_route_t *route)
{
//TODO Temp solution, create a local pthread
	pthread_t pthread;
printf("!start socket listener\n");
	int i = pthread_create(&pthread, NULL, route->in_connector->content.sock.consumer_callback, (void*)route);
	pthread_join(pthread, NULL);
}

void open_input_socket_connector(const int fd)
{
	
}

/** Create an initialized in connector.
 */
static rt_in_connector_t* init_in_connector(in_connector_t *connector)
{
	rt_in_connector_t *res = (rt_in_connector_t*)malloc(sizeof(rt_in_connector_t));
	res->type = connector->type;
	switch(connector->type){
	case TYPE_SOCKET:
		res->content.sock.port = connector->content.sock.port;
		res->content.sock.fd = create_input_socket_connector(connector->content.sock);
printf("Init callback, fd:%d\n", res->content.sock.fd);
		res->content.sock.consumer_callback = socket_connector;
		break;
	case TYPE_FILE:
		res->content.file.filename = connector->content.file.filename;
		res->content.file.file = fopen(connector->content.file.filename, "r");
		break;
	default:
		res = NULL;
		break;
	}
	return res;
}

static rt_out_connector_t* init_out_connector(out_connector_t *connector)
{
	rt_out_connector_t *res = (rt_out_connector_t*)malloc(sizeof(rt_out_connector_t));
	res->type = connector->type;
	switch(connector->type){
	case TYPE_SOCKET:
		res->content.sock.port = connector->content.sock.port;
		res->content.sock.fd = create_input_socket_connector(connector->content.sock);
//		res->content.sock.consumer_callback = socket_connector;
		break;
	case TYPE_FILE:
			//TODO callback
		res->content.file.filename = connector->content.file.filename;
		res->content.file.file = fopen(connector->content.file.filename, "w");
printf("Set up file\n");
		res->content.file.output_callback = file_writer;
		break;
	}
printf("End init2\n");
	return res;
}

static rt_route_t* init_route(route_t *route)
{
	rt_route_t *res = (rt_route_t*)malloc(sizeof(rt_route_t));
	res->in_connector = init_in_connector(&(route->in_connector));
	res->out_connections = route->out_connections;
	res->out_connectors = (rt_out_connector_t**)malloc(sizeof(rt_out_connector_t*) * (route->out_connections +1));
	res->forward_mode = route->forward_mode;
	int j;
	for (j=0; j < res->out_connections; j++){
		res->out_connectors[j] = init_out_connector(route->out_connectors[j]);
	}
	res->out_connectors[j] = NULL;
	return res;
}


static void start_in_listener(rt_route_t *route)
{
printf("!!start listeners\n");
	switch(route->in_connector->type){
	case TYPE_SOCKET:
		start_socket_listener(route);
		break;
	case TYPE_FILE:
		//TODO
		break;
	case TYPE_WORKFLOW:
printf("start_listeners_WORFLOW not implemented\n");
		break;
	}
}

static void join_all_listeners(rt_route_t **rt_routes)
{
printf("Joining all listeners\n");
}

void init_all_routes(route_t** routes, const int max_routes)
{
	int i = 0;
	rt_route_t **rt_routes = (rt_route_t**)malloc(sizeof(rt_route_t*) * max_routes);
	for(i=0; i < max_routes; i++){
		rt_routes[i] = init_route(routes[i]);
	}

	for(i=0; i < max_routes; i++){
		start_in_listener(rt_routes[i]);
	}

	join_all_listeners(rt_routes);
}

