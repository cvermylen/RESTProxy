#ifndef ROUTE_INSTANCE_H
#define ROUTE_INSTANCE_H

#include "route.h"
#include "http/http_connection.h"

typedef struct {
    route_t* route;
    ri_connection_t** connections;
} route_instance_t;

route_instance_t* start_route(route_t *route);

#endif