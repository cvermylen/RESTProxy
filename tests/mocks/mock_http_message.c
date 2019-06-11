#include "../../src/http/http_message.h"

int mock_called_new_http_message;
http_message_t* mock_result_new_http_message;
http_message_t* new_http_message (int (*feed_data) (void* conn_params, char* dest_buffer, int max_buffer_size), void* connection_params, int buffer_size)
{
    mock_called_new_http_message += 1;
}

int mock_called_http_message_decode_request_type;
int mock_result_http_message_decode_request_type;
int http_message_decode_request_type (http_message_t* msg)
{
    mock_called_http_message_decode_request_type += 1;
    return mock_result_http_message_decode_request_type;
}

int mock_called_http_message_decode_response_type;
int mock_result_http_message_decode_response_type;
int http_message_decode_response_type (http_message_t* msg)
{
    mock_called_http_message_decode_response_type += 1;
    return mock_result_http_message_decode_response_type;
}

int mock_called_read_next_buffer_from_source;
int mock_result_read_next_buffer_from_source;
int read_next_buffer_from_source (http_message_t* msg)
{
    mock_called_read_next_buffer_from_source += 1;;
    return mock_result_read_next_buffer_from_source;
}

int mock_called_http_message_free;
void http_message_free (http_message_t* msg)
{
    mock_called_http_message_free += 1;
}

int mock_called_decode_http_message_header;
long decode_http_message_header_param1;
void decode_http_message_header (http_message_t* msg)
{
    mock_called_decode_http_message_header += 1;
    decode_http_message_header_param1 = (long)msg;
}

int mock_called_http_message_receive_body;
void http_message_receive_body (http_message_t* msg)
{
    mock_called_http_message_receive_body += 1;
}

int mock_called_send_next_buffer_to_destination;
void send_next_buffer_to_destination (int(*send_data)(), void* connection_params, http_message_t* msg, char move_pointer)
{
    mock_called_send_next_buffer_to_destination += 1;
}