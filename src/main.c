#include "shared_buffers.h"
#include "route_instance.h"
#include "route_def.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

ri_route_t* primary_route;

void* workflow_config()
{
	return NULL;
}

ri_route_t* create_default_route(const int port, const char* file_name1, const char* file_name2)
{
	ri_route_t* route = create_runtime_route(port, FORWARD_MODE_ASYNC, 1);
	add_out_file_connector(route, 0, file_name1, FLOW_BIDIRECTIONAL);
	//set_file_out_connector(route, 1, file_name2, NULL);
	//set_workflow_connector(route, FLOW_SYNC_ANSWER, 1, "../../sample/test_wkf_file", NULL);
	return route;
}

void close_route()
{
printf("Control-C\n");
	release_runtime_route(primary_route);
}

int main(int argc, char* argv[])
{
	init_shared_buffers();
	primary_route = create_default_route(8080, "router_test_file.txt", "router_test_file2.txt");
	signal(SIGINT, close_route);
	start_in_listener(primary_route);
}
