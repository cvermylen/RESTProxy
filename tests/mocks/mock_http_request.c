#include "../../src/http/http_request.h"
#include <stdlib.h>

int mock_called_new_http_request;
request_t* new_http_request (int(*open_connection) (void* conn_params),
                             int(*feeder)(),
                             int (*send_data) (void* conn_params, char* dest_buffer, int max_buffer_size),
                             int(*close_connection) (void* conn_params),
                             void* connection_params)
                             {
    mock_called_new_http_request += 1;
    request_t* request = (request_t*) malloc (sizeof(request_t));
    return request;
}

int mock_called_receive_new_request_from_client;
void receive_new_request_from_client (request_t *request)
{
    mock_called_receive_new_request_from_client += 1;
}

int mock_called_process_request_message_body;
void process_request_message_body (request_t* request)
{
    mock_called_process_request_message_body += 1;
}

int mock_called_get_request_connection_keep_alive;
int mock_result_get_request_connection_keep_alive;
int get_request_connection_keep_alive (request_t* request)
{
    mock_called_get_request_connection_keep_alive += 1;
    return mock_result_get_request_connection_keep_alive;
}

int mock_called_close_client_connection;
void close_client_connection (request_t* request)
{
    mock_called_close_client_connection += 1;
}

int mock_called_release_request;
request_t* mock_result_release_request;
request_t* release_request (request_t *d)
{
    mock_called_release_request += 1;
    return mock_result_release_request;
}