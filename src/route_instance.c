
#include "route_instance.h"


route_instance_t* start_route(route_t *route)
{
	pthread_t pthread;
	route_instance_t* ri = (route_instance_t*) malloc (sizeof(route_instance_t));
	ri->route = route;
	ri->connections = (ri_connection_t**) malloc (sizeof(ri_connection_t*) * 1);
	ri_connection_t* conn = new_http_connection(route);
	ri->connections[0] = conn;
	int i = pthread_create (&pthread, NULL, (void* (*) (void*))run_session, conn);
	pthread_join(pthread, NULL);
	return ri;
}
