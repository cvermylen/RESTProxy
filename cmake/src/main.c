#include "shared_buffers.h"
#include "routing_handler.h"
#include "routing_params.h"
#include <stdio.h>
#include <stdlib.h>

void* workflow_config()
{
	return NULL;
}

route_t* create_default_route(const int port, const char* file_name1, const char* file_name2)
{
	int route = alloc_route(1, FORWARD_MODE_ASYNC);
	set_socket_in_connector(route, MODE_TCP, port, NULL);
	set_file_out_connector(route, 0, file_name1, NULL);
	//set_file_out_connector(route, 1, file_name2, NULL);
	//set_workflow_connector(route, FLOW_SYNC_ANSWER, 1, "../../sample/test_wkf_file", NULL);
	return get_route(route);
}

int main(int argc, char* argv[])
{
	init_shared_buffers();
	route_t* primary = create_default_route(8080, "router_test_file.txt", "router_test_file2.txt");
	route_t **routes = (route_t**)malloc(sizeof(route_t*));
	routes[0] = primary;
	init_all_routes(routes, 1);
}
