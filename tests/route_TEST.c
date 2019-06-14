#include "../src/route.h"
#include "../src/route_def.h"
#include "../src/socket/socket_connector.h"
#include <criterion/criterion.h>

extern in_connector_t *create_runtime_in_connector (int type, int port);
extern int mock_called_create_runtime_sock_connector;
extern ri_sock_connector_t* mock_result_create_runtime_sock_connector;;
Test (create_runtime_in_connector, SOCKET)
{
    mock_called_create_runtime_sock_connector = 0;
    mock_result_create_runtime_sock_connector = (ri_sock_connector_t*)4l;
    in_connector_t* c = create_runtime_in_connector (TYPE_SOCKET, 0);

    cr_assert_not_null (c, "Should not have returned a null value:'create_runtime_sock_connector'");
    cr_assert (1 == mock_called_create_runtime_sock_connector, "Should have called 'create_runtime_sock_connector'."
                                                               "Actual was:%d", mock_called_create_runtime_sock_connector);
    cr_assert_not_null (c->connection_params);
    cr_assert_not_null (c->feed_data);
    cr_assert_not_null (c->send_data);
    cr_assert_not_null (c->open_connection);
    cr_assert_not_null (c->close_connection);
}

Test (create_runtime_in_connector, unknown)
{
    mock_called_create_runtime_sock_connector = 0;

    in_connector_t* c = create_runtime_in_connector (99, 0);

    cr_assert_null(c, "Should return NULL for an unknown connector type");
    cr_assert (0 == mock_called_create_runtime_sock_connector, "Should not have called 'create_runtime_sock_connector'."
                                                               "Actual was:%d", mock_called_create_runtime_sock_connector);
}

Test (create_route, call_all_functions)
{
    mock_called_create_runtime_sock_connector = 0;

    route_t* route = create_route (2, 5, 9);

    cr_assert_not_null (route, "Should always have created a route");
    cr_assert (1 == mock_called_create_runtime_sock_connector, "Should have created socket connectors."
                                                               "Actual was:%d", mock_called_create_runtime_sock_connector);
    cr_assert (5 == route->forward_mode, "Should have assigned correctly the forward_mode. Actual was:%d", route->forward_mode);
    cr_assert (9 == route->out_connections, "Should have assigned correctly the number of connections."
                                            "Actual was:%d", route->out_connections);
    cr_assert_not_null (route->out_connectors, "Should have initialized the server connections");
}

extern out_connector_t* create_runtime_out_sock_connector (int flow, char* hostname, int port);
Test (create_runtime_out_sock_connector, call_all_functions)
{

    out_connector_t* c = create_runtime_out_sock_connector (3, "host", 8);

    cr_assert_not_null (c, "Should always create a connector");
    cr_assert (strcmp("host", ((ri_sock_connector_t*)c->connection_params)->server_name) == 0, "Should have propagated the server name."
                                                     "Actual is:%s", ((ri_sock_connector_t*)c->connection_params)->server_name);
    cr_assert (8 == ((ri_sock_connector_t*)c->connection_params)->port, "Should have propagated the server port."
                                                                        "Actual was:%d", ((ri_sock_connector_t*)c->connection_params)->port);
    cr_assert (TYPE_SOCKET == c->type, "Expected to be a socket, Actual was:%d", c->type);
    cr_assert (3 == c->flow, "Expected to have propagated the flow:%d", c->flow);
    cr_assert_not_null (c->open_connection, "'open_connection' should not be null");
    cr_assert_not_null (c->send_data, "'send_data' should not be null");
    cr_assert_not_null (c->receive_data, "'receive_data' should not be null");
    cr_assert_not_null (c->close_connection, "'close_connection' should not be null");
}