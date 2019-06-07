#include "../src/route_def.h"
#include "../src/http/http_connection.h"
#include "../src/route_instance.h"
#include <criterion/criterion.h>
#include <stdlib.h>
#include "mocks/mock_socket.h"

ri_route_t *create_mock_route(int port)
{
	ri_route_t *route = (ri_route_t*) malloc(sizeof(ri_route_t));
	route->in_connector = (ri_in_connector_t*)malloc(sizeof(ri_in_connector_t));
	route->in_connector->type = TYPE_SOCKET;
	return route;
}

Test(conn, happy)
{
	ri_connection_t* ret;
	
	ret = wait_4_connection_request(create_mock_route(2000));
	cr_assert(function_called == MOCK_SOCKET_ACCEPT, "Should have called accept");
	cr_assert(ret->fd > 0, "Requesting port 2000 should have passed");
	free_mock_sockets();
}

Test(conn, negative)
{
	ri_connection_t* ret;
	ret = wait_4_connection_request(create_mock_route(-1));
	cr_assert(ret->fd <= 0, "Requesting a negative port should have failed:%d", ret->fd);
	free_mock_sockets();
}

Test(conn, bind_negative)
{
	int s = bind_port(0);
	cr_assert(s < 0, "Cannot bind on negative or port 0");
}

Test(conn, bind)
{
	sock_rval = 3;

	int s = bind_port(8080);
	cr_assert(function_called & MOCK_SOCKET_SOCKET, "Should have called 'socket':%d", function_called);
	cr_assert(function_called & MOCK_SOCKET_OPT, "Should have called 'setsockopt':%d", function_called);
	cr_assert(function_called & MOCK_SOCKET_BIND, "Should have called 'bind': %d", function_called);
	cr_assert(function_called & MOCK_SOCKET_LISTEN, "Should have called 'listen': %d", function_called);
	cr_assert(s > 0,"Should have returned valid fd number, not: %d", s);
}
