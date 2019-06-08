#include <string.h>
#include "route.h"
#include "route_def.h"
#include "socket/socket_connector.h"
#include "file_connector.h"

in_connector_t *create_runtime_in_connector(const int type, const int port)
{
    printf("create_runtime_in_connector\n");
    in_connector_t *res = (in_connector_t*)malloc(sizeof(in_connector_t));
    res->type = type;
    switch(type){
        case TYPE_SOCKET:
            res->connection_params = create_runtime_sock_connector (port);
            res->feed_data = (int (*) (void*, char*, int))read_from_socket;
            res->send_data = (int (*) (void*, char*, int))sock_write;
            res->open_connection = (int (*) (void*))open_socket_connector;
            res->close_connection = (int (*) (void*))close_socket;
            break;
        case TYPE_FILE:
            // res->content.file = create_runtime_file_connector(&(conn->content.file));
            break;
    }
    return res;
}

route_t *create_route(const int port, const int mode, const int num_connectors)
{
    printf("create_route\n");
    route_t *res = (route_t*)malloc(sizeof(route_t));
    res->out_connections = num_connectors;
    res->forward_mode = mode;
    res->in_connector = create_runtime_in_connector(TYPE_SOCKET, port);
    res->out_connectors = (out_connector_t**)malloc(sizeof(out_connector_t*) * num_connectors);
    return res;
}

out_connector_t* create_runtime_out_sock_connector(const int flow, const char* hostname, const int port)
{
    ri_sock_connector_t* res = (ri_sock_connector_t*)malloc(sizeof(ri_sock_connector_t));
    res->port = port;
    res->server_name = (char*)malloc(sizeof(char) * strlen(hostname));
    strcpy(res->server_name, hostname);

    out_connector_t* conn = (out_connector_t*)malloc(sizeof(out_connector_t));
    conn->type = TYPE_SOCKET;
    conn->flow = flow;
    conn->connection_params = res;
    conn->open_connection = (int (*) (void*))open_server_socket_connector;
    conn->send_data = (int (*) (void*, char*, int))sock_write;
    conn->receive_data = (int (*) (void*, char*, int))read_from_socket;
    conn->close_connection = (int (*) (void*))close_socket;
    return conn;
}

out_connector_t *create_runtime_out_file_connector(const int flow, const char* filename)
{
    printf("create_runtime_file_connector\n");
    ri_file_connector_t * res = (ri_file_connector_t*)malloc(sizeof(ri_file_connector_t));
    res->filename = (char*)malloc((strlen(filename) + 1) * sizeof(char));
    strcpy(res->filename, filename);

    out_connector_t* conn = (out_connector_t*)malloc(sizeof(out_connector_t));
    conn->connection_params = res;
    conn->type = TYPE_FILE;
    conn->flow = flow;
    conn->open_connection = (int (*) (void*))open_file;
    conn->send_data = (int (*) (void*, char*, int))file_writer;
    conn->receive_data = (int (*) (void*, char*, int))file_reader;
    conn->close_connection = (int (*) (void*))close_file;
    return conn;
}

void release_runtime_in_connector (in_connector_t* conn)
{
    free (conn);
}

void release_runtime_out_connector(out_connector_t *conn)
{
    free(conn);
}

void add_out_file_connector(route_t* route, const int index, const char* filename, const int flow)
{
    route->out_connectors[index] = create_runtime_out_file_connector(flow, filename);
}

void add_out_sock_connector(route_t* route, const int index, const char* hostname, const int port, const int flow)
{
    route->out_connectors[index] = create_runtime_out_sock_connector(flow, hostname, port);
}

void free_route (route_t* route)
{
    release_runtime_in_connector (route->in_connector);
    for (int i=0; i < route->out_connections; i++) {
        release_runtime_out_connector(route->out_connectors[i]);
    }
}
