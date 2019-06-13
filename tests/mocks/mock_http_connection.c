#include "../../src/http/http_connection.h"
#include "mock_defs.h"

int mock_called_new_http_connection;
ri_connection_t* mock_result_new_http_connection;
long mock_param_1_new_http_connection[MAX_CALLS_SAME_FUNCTION];
ri_connection_t* new_http_connection (route_t* route)
{
    mock_param_1_new_http_connection[mock_called_new_http_connection] = (long)route;
    mock_called_new_http_connection += 1;
    return mock_result_new_http_connection;
}

int mock_called_open_connection;
int mock_result_open_connection;
long mock_param_1_open_connection[MAX_CALLS_SAME_FUNCTION];
int open_connection(ri_connection_t *conn)
{
    mock_param_1_open_connection[mock_called_open_connection] = (long)conn;
    mock_called_open_connection += 1;
    return mock_result_open_connection;
}

int mock_called_run_session;
void* mock_result_run_session;
long mock_param_1_run_session[MAX_CALLS_SAME_FUNCTION];
void* run_session (ri_connection_t* conn)
{
    mock_param_1_run_session[mock_result_open_connection] = (long)conn;
    mock_called_run_session += 1;
    return mock_result_run_session;
}

int mock_called_close_connection;
long mock_param_1_close_connection[MAX_CALLS_SAME_FUNCTION];
void close_connection (ri_connection_t *conn)
{
    mock_param_1_close_connection[mock_called_close_connection] = (long) conn;
    mock_called_close_connection += 1;
}

int mock_called_release_runtime_route;
long mock_param_1_release_runtime_route[MAX_CALLS_SAME_FUNCTION];
void release_runtime_route(ri_connection_t* conn)
{
    mock_param_1_release_runtime_route[mock_called_release_runtime_route] = (long) conn;
    mock_called_release_runtime_route += 1;
}

