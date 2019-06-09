#include "../../src/http/http_request.h"
#include <criterion/criterion.h>

extern int mock_called_new_http_message;
Test (request, new)
{
    mock_called_new_http_message = 0;
    request_t* r = new_http_request(NULL, NULL, NULL, NULL, NULL);

    cr_assert_not_null(r, "'new_http_request' cannot return NULL value");
    cr_assert (1 == mock_called_new_http_message, "Should have called 'new_http_message'. "
                                                  "Actual was:%d", mock_called_new_http_message);
}

extern int mock_called_http_message_free;
Test (release_request, call)
{
    request_t* r = new_http_request(NULL, NULL, NULL, NULL, NULL);
    mock_called_http_message_free = 0;

    r = release_request (r);

    cr_assert_null (r, "Should have returned NULL");
    cr_assert (1 == mock_called_http_message_free, "Should have called 'http_message_free'. "
                                                   "Actual was:%d", mock_called_http_message_free);
}

int mock_call_open_connection;
int mock_open_connection (void* params)
{
    mock_call_open_connection++;
    return 1;
}
extern int mock_called_read_next_buffer_from_source;
extern int mock_called_http_message_decode_request_type;
extern int mock_called_decode_http_message_header;
Test (receive_new_request_from_client, call)
{
    request_t* request = new_http_request(mock_open_connection, NULL, NULL, NULL, NULL);
    mock_call_open_connection = 0;
    mock_called_read_next_buffer_from_source = 0;
    mock_called_http_message_decode_request_type = 0;
    mock_called_decode_http_message_header = 0;

    receive_new_request_from_client (request);

    cr_assert (1 == mock_call_open_connection, "Should have called 'mock_call_open_connection', "
                                               "Actual was: %d", mock_call_open_connection);
    cr_assert (2 == mock_called_read_next_buffer_from_source, "Should have called 'read_next_buffer_from_source'. "
                                                            "Actual was:%d", mock_called_read_next_buffer_from_source);
    cr_assert (1 == mock_called_http_message_decode_request_type, "Should have called 'http_message_decode_request_type'."
                                                                "Actual was:%d", mock_called_http_message_decode_request_type);
    cr_assert (1 == mock_called_decode_http_message_header, "Should have called 'decode_http_message_header'."
                                                          "Actual was:%d", mock_called_decode_http_message_header);
    release_request (request);
}

extern int mock_called_decode_keep_alive;
Test (get_request_connection_keep_alive, call)
{
    request_t* request = new_http_request(NULL, NULL, NULL, NULL, NULL);
    mock_called_decode_keep_alive = 0;

    int res = get_request_connection_keep_alive (request);

    cr_assert (1 == mock_called_decode_keep_alive, "Should have call decode_keep_alive. Actual was:%d", mock_called_decode_keep_alive);
    release_request (request);
}

int mock_call_close_connection;
int mock_close_connection (void* params)
{
    mock_call_close_connection++;
    return 1;
}
Test (close_client_connection, call)
{
    request_t* request = new_http_request(NULL, NULL, NULL, mock_close_connection, NULL);
    mock_call_close_connection = 0;

    close_client_connection (request);

    cr_assert (1 == mock_call_close_connection, "Should have called 'mock_close_connection'."
                                                "Actual was:%d", mock_call_close_connection);
    release_request (request);
}
