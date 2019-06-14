#include <criterion/criterion.h>
#include <string.h>

#include "../src/route_instance.h"
#include "../src/route.h"
#include "../src/route_def.h"
#include "../src/socket/socket_connector.h"
#include "../src/file_connector.h"

static ri_sock_connector_t* mock_sock_connector()
{
	ri_sock_connector_t* res = (ri_sock_connector_t*)malloc(sizeof(ri_sock_connector_t));
	res->port = 1;
	res->mode = 2;
	return res;
}

static ri_sock_connector_t* mock_ri_sock_connector()
{
	ri_sock_connector_t* res = (ri_sock_connector_t*)malloc(sizeof(ri_sock_connector_t));
	res->port = 1;
	res->mode = 2;
	res->fd = 3;
	return res;
}

static ri_file_connector_t* mock_file_connector()
{
	ri_file_connector_t* res = (ri_file_connector_t*)malloc(sizeof(ri_file_connector_t));
	res->filename = (char*)malloc(sizeof(char));
	res->filename[0] = '\0';
	return res;
}

static ri_file_connector_t* mock_ri_file_connector()
{
	ri_file_connector_t* res = (ri_file_connector_t*)malloc(sizeof(ri_file_connector_t));
	res->filename = (char*)malloc(sizeof(char));
	res->filename[0] = '\0';
	res->file = (FILE*)malloc(sizeof(FILE));
	return res;
}

static out_connector_t* mock_ri_out_connector(const int type)
{
	out_connector_t* res = (out_connector_t*)malloc(sizeof(out_connector_t));
	res->type = type;
	/*switch(type){
	case TYPE_SOCKET:
		res->content.sock = mock_ri_sock_connector();
		break;
	case TYPE_FILE:
		res->content.file = mock_ri_file_connector();
		break;
	case TYPE_WORKFLOW:
		res->content.workflow = NULL;
		break;
	}*/
	return res;
}

static out_connector_t* mock_out_connector(const int type)
{
	out_connector_t* res = (out_connector_t*)malloc(sizeof(out_connector_t));
	res->type = type;
	switch(type){
	case TYPE_SOCKET:
		//res->content.sock = mock_sock_connector();
		break;
	case TYPE_FILE:
		//res->content.file = mock_file_connector();
		break;
	}
	return res;
}

static in_connector_t* mock_ri_in_connector(const int type)
{
	in_connector_t* res = (in_connector_t*)malloc(sizeof(in_connector_t));
	res->type = type;
	/*switch(type){
	case TYPE_SOCKET:
		res->content.sock = mock_ri_sock_connector();
		break;
	case TYPE_FILE:
		res->content.file = mock_ri_file_connector();
		break;
	}*/
	return res;
}

static route_t* mock_route(const int out_conn)
{
	route_t* route = (route_t*)malloc(sizeof(route_t));
	route->in_connector = mock_ri_in_connector(TYPE_SOCKET);
	route->out_connections = out_conn;
	route->out_connectors = (out_connector_t**)malloc(sizeof(out_connector_t*) * out_conn);
	for(int i = 0; i < out_conn; i++){
		route->out_connectors[i] = mock_ri_out_connector(TYPE_FILE);
	}
	return route;
}

Test (start_route, propagate_calls)
{
    route_instance_t* ri = start_route (mock_route(1));

    cr_assert_not_null (ri);
}
