#include "log.h"
#include "routing_params.h"
#include <stdlib.h>
#include <string.h>

route_t* routes[MAX_ROUTES];

int available_routes()
{
	int res = 0;
	int i = 0;
	for(; i < MAX_ROUTES; i++){
		if(!routes[i])res++;
	}
	return res;
}

int alloc_route(int out_connections, int forward_mode)
{
	int i = 0;
	for(; i < MAX_ROUTES && routes[i] != NULL; i++);
	if(i < MAX_ROUTES){
		routes[i] = (route_t*)malloc(sizeof(route_t));
		routes[i] -> out_connections = out_connections;
		routes[i] -> out_connectors = NULL;
		routes[i] -> forward_mode = forward_mode;
		if(out_connections > 0) {
			routes[i] -> out_connectors = (out_connector_t**)malloc(sizeof(out_connector_t*) * out_connections);
			int j = 0;
			for(; j < out_connections; j++){
				routes[i] -> out_connectors[j] = (out_connector_t*)malloc(sizeof(out_connector_t));
				routes[i] -> out_connectors[j] -> type = -1;
			}
		}
		return i;
	}else{
		return -1;
	}
}

route_t* get_route(int route_id)
{
	if(route_id >= 0 && route_id < MAX_ROUTES){
		return routes[route_id];
	}else{
		return NULL;
	}
}

void set_file_out_connector(const int route_id, const int connector_id, const char* filename, void(*callback)())
{
	route_t* l = get_route(route_id);
	if(connector_id < l->out_connections){
		l->out_connectors[connector_id]->type = TYPE_FILE;
		l->out_connectors[connector_id]->content.file.filename = (char*)malloc(sizeof(char)*strlen(filename)+1);
		strcpy(l->out_connectors[connector_id]->content.file.filename, filename);
	}else{
		log_fatal("Attempt to create more connectors than previously declared. Route id %d, connections %d\n", route_id, connector_id);
	}
}

void set_socket_in_connector(const int route_id, const int mode, const int port, void(**callback)())
{
	route_t* l = get_route(route_id);
	l->in_connector.type = TYPE_SOCKET;
	l->in_connector.content.sock.mode = mode;
	l->in_connector.content.sock.port = port;
}

void set_workflow_out_connector(const int route_id, const int connector_id, char* filename, void(**callback)())
{
	route_t* r = get_route(route_id);
	if(connector_id < r->out_connections){
		r->out_connectors[connector_id]->type = TYPE_WORKFLOW;
		r->out_connectors[connector_id]->content.workflow.filename = filename;
	}else{
		log_fatal("Attempt to create more connectors than previously declared. Route id %d, connections %d\n", route_id, connector_id);
	}
}
	

static void free_file_connector(out_connector_t* connector)
{
	free(connector->content.file.filename);
}

static void free_connector(route_t* route, const int conn_id)
{
	out_connector_t* conn = route->out_connectors[conn_id];
	if(conn != NULL){
		switch(conn->type){
		case TYPE_FILE:
			free_file_connector(conn);
			break;
		case TYPE_SOCKET:
			break;
		}
	}
	free(conn);
	route->out_connectors[conn_id] = NULL;
}

void free_route(int route_id)
{
	if(route_id >= 0 && route_id < MAX_ROUTES && routes[route_id] != NULL){
		route_t* route = get_route(route_id);
		for(int i=0; i < route->out_connections; i++)
			free_connector(route, i);
		free(route->out_connectors);
		free(route);
		routes[route_id] = NULL;
	}
}

void free_all_routes()
{
	int i;
	for(i = 0; i < MAX_ROUTES; i++) {
		free_route(i);
	}
}
