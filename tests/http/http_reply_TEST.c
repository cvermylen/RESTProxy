#include "../../src/http/http_reply.h"
#include "../../src/route_def.h"
#include <criterion/criterion.h>

Test (request, new)
{
    reply_t* r = create_reply( NULL, NULL, NULL, NULL, NULL);

    cr_assert_not_null(r, "'new_http_request' cannot return NULL value");
}

extern int mock_called_http_message_free;
Test (release_reply, call)
{
    reply_t* r = create_reply( NULL, NULL, NULL, NULL, NULL);
    mock_called_http_message_free = 0;

    r = release_reply (r);

    cr_assert_null (r, "Should have returned NULL");
    cr_assert (1 == mock_called_http_message_free, "Should have called 'http_message_free'."
                                                   "Actual was:%d", mock_called_http_message_free);
}

extern int mock_called_send_next_buffer_to_destination;
struct mock_envelope {
    http_message_t* requestMessage;
    reply_t* reply;
    char last_server_in_list;
};
extern void* send_request (struct mock_envelope* env);
Test (send_request, call)
{
    struct mock_envelope* env  = (struct mock_envelope*) malloc (sizeof(struct mock_envelope));
    env->reply = create_reply(NULL, NULL, NULL, NULL, NULL);
    mock_called_send_next_buffer_to_destination = 0;

    send_request (env);

    cr_assert (1 == mock_called_send_next_buffer_to_destination, "Should have called 'send_next_buffer_to_destination'."
                                                                 "Actual was:%d", mock_called_send_next_buffer_to_destination);
    release_reply (env->reply);
    free (env);
}

int mock_call_open_connection;
int mock_open_connection (void* params)
{
    mock_call_open_connection++;
    return 1;
}
Test (connect_to_server, call)
{
    reply_t* reply = create_reply ( mock_open_connection, NULL, NULL, NULL, NULL);
    mock_call_open_connection = 0;

    connect_to_server (reply);

    cr_assert (1 == mock_call_open_connection, "Should have been called 'mock_open_connection'."
                                               "Actual was:%d", mock_call_open_connection);

    release_reply (reply);
}

Test (reply_to_client, call)
{
    reply_t* reply = create_reply( NULL, NULL, NULL, NULL, NULL);
    mock_called_send_next_buffer_to_destination = 0;

    reply_to_client (reply);

    cr_assert (1 == mock_called_send_next_buffer_to_destination, "Should have been called, 'send_next_buffer_to_destination'."
                                                     "Actual was:%d", mock_called_send_next_buffer_to_destination);
    release_reply (reply);
}

extern void accept_reply_from_server(reply_t *reply);
extern int mock_called_read_next_buffer_from_source;
extern int mock_called_http_message_decode_response_type;
extern int mock_result_http_message_decode_response_type;
Test (accept_reply_from_server, call)
{
    reply_t* reply = create_reply( NULL, NULL, NULL, NULL, NULL);
    mock_called_read_next_buffer_from_source = 0;
    mock_called_http_message_decode_response_type = 0;
    mock_result_http_message_decode_response_type = 5;
    reply->type = 2;

    accept_reply_from_server (reply);

    cr_assert (1 == mock_called_read_next_buffer_from_source, "Should have been called 'read_next_buffer_from_source'."
                                                              "Actual was:%d", mock_called_read_next_buffer_from_source);
    cr_assert (1 == mock_called_http_message_decode_response_type, "Should have been called 'http_message_decode_response_type'."
                                                                   "Actual was:%d", mock_called_http_message_decode_response_type);
    cr_assert (5 == reply->type, "Should have been set to '5': reply->type. Instead value is:%d", reply->type);

    release_reply (reply);
}

extern int mock_called_decode_http_message_header;
extern int mock_called_http_message_receive_body;
extern void receive_reply(reply_t *reply);
Test (receive_reply, call)
{
    reply_t* reply = create_reply( NULL, NULL, NULL, NULL, NULL);
    mock_called_decode_http_message_header = 0;
    mock_called_http_message_receive_body = 0;
    mock_called_read_next_buffer_from_source = 0;
    mock_called_http_message_decode_response_type = 0;

    receive_reply (reply);

    cr_assert (1 == mock_called_decode_http_message_header, "Should have been called 'decode_http_message_header'."
                                                              "Actual was:%d", mock_called_decode_http_message_header);
    cr_assert (1 == mock_called_http_message_receive_body, "Should have been called 'http_message_receive_body'."
                                                              "Actual was:%d", mock_called_http_message_receive_body);
    cr_assert (1 == mock_called_read_next_buffer_from_source, "Should have been called 'read_next_buffer_from_source'."
                                                              "Actual was:%d", mock_called_read_next_buffer_from_source);
    cr_assert (1 == mock_called_http_message_decode_response_type, "Should have been called 'http_message_decode_response_type'."
                                                                   "Actual was:%d", mock_called_http_message_decode_response_type);
    release_reply (reply);
}

extern void send_request_receive_reply (struct mock_envelope* env);
Test (send_request_receive_reply, call)
{
    struct mock_envelope* env  = (struct mock_envelope*) malloc (sizeof(struct mock_envelope));
    env->reply = create_reply(mock_open_connection, NULL, NULL, NULL, NULL);
    mock_call_open_connection = 0;
    mock_called_send_next_buffer_to_destination = 0;
    mock_called_decode_http_message_header = 0;
    mock_called_http_message_receive_body = 0;
    mock_called_read_next_buffer_from_source = 0;
    mock_called_http_message_decode_response_type = 0;

    send_request_receive_reply (env);

    cr_assert (1 == mock_call_open_connection, "Should have been called 'mock_open_connection'."
                                               "Actual was:%d", mock_call_open_connection);
    cr_assert (1 == mock_called_send_next_buffer_to_destination, "Should have called 'send_next_buffer_to_destination'."
                                                                 "Actual was:%d", mock_called_send_next_buffer_to_destination);
    cr_assert (1 == mock_called_decode_http_message_header, "Should have been called 'decode_http_message_header'."
                                                            "Actual was:%d", mock_called_decode_http_message_header);
    cr_assert (1 == mock_called_http_message_receive_body, "Should have been called 'http_message_receive_body'."
                                                           "Actual was:%d", mock_called_http_message_receive_body);
    cr_assert (1 == mock_called_read_next_buffer_from_source, "Should have been called 'read_next_buffer_from_source'."
                                                              "Actual was:%d", mock_called_read_next_buffer_from_source);
    cr_assert (1 == mock_called_http_message_decode_response_type, "Should have been called 'http_message_decode_response_type'."
                                                                   "Actual was:%d", mock_called_http_message_decode_response_type);
    release_reply (env->reply);
    free (env);
}

int mock_call_close_connection;
int mock_close_connection (void* params)
{
    mock_call_close_connection++;
    return 1;
}
Test (close_server_connection, call)
{
    reply_t* reply = create_reply( NULL, NULL, NULL, mock_close_connection, NULL);
    mock_call_close_connection = 0;

    close_server_connection (reply);

    cr_assert (1 == mock_call_close_connection, "Should have called 'mock_close_connection'."
                                                "Actual was:%d", mock_call_close_connection);
    release_reply (reply);
}

Test (send_request_to_server_and_wait_reply, synchrone)
{
    reply_t* reply = create_reply(mock_open_connection, NULL, NULL, NULL, NULL);
    mock_call_open_connection = 0;
    mock_called_send_next_buffer_to_destination = 0;
    mock_called_decode_http_message_header = 0;
    mock_called_http_message_receive_body = 0;
    mock_called_read_next_buffer_from_source = 0;
    mock_called_http_message_decode_response_type = 0;

    send_request_to_server_and_wait_reply (NULL, reply, 1, FORWARD_MODE_SEQ);

    cr_assert (1 == mock_call_open_connection, "Should have been called 'mock_open_connection'."
                                               "Actual was:%d", mock_call_open_connection);
    cr_assert (1 == mock_called_send_next_buffer_to_destination, "Should have called 'send_next_buffer_to_destination'."
                                                                 "Actual was:%d", mock_called_send_next_buffer_to_destination);
    cr_assert (1 == mock_called_decode_http_message_header, "Should have been called 'decode_http_message_header'."
                                                            "Actual was:%d", mock_called_decode_http_message_header);
    cr_assert (1 == mock_called_http_message_receive_body, "Should have been called 'http_message_receive_body'."
                                                           "Actual was:%d", mock_called_http_message_receive_body);
    cr_assert (1 == mock_called_read_next_buffer_from_source, "Should have been called 'read_next_buffer_from_source'."
                                                              "Actual was:%d", mock_called_read_next_buffer_from_source);
    cr_assert (1 == mock_called_http_message_decode_response_type, "Should have been called 'http_message_decode_response_type'."
                                                                   "Actual was:%d", mock_called_http_message_decode_response_type);
    release_reply (reply);
}

extern int mock_called_pthread_create;
Test (send_request_to_server_and_wait_reply, async)
{
    reply_t* reply = create_reply(mock_open_connection, NULL, NULL, NULL, NULL);
    mock_call_open_connection = 0;
    mock_called_send_next_buffer_to_destination = 0;
    mock_called_decode_http_message_header = 0;
    mock_called_http_message_receive_body = 0;
    mock_called_read_next_buffer_from_source = 0;
    mock_called_http_message_decode_response_type = 0;

    send_request_to_server_and_wait_reply (NULL, reply, 1, FORWARD_MODE_ASYNC);

    cr_assert (1 == mock_called_pthread_create, "Should have called 'pthread_create'. Actual was:%d", mock_called_pthread_create);
    cr_assert (0 == mock_call_open_connection, "Should have been called 'mock_open_connection'."
                                               "Actual was:%d", mock_call_open_connection);
    cr_assert (0 == mock_called_send_next_buffer_to_destination, "Should have called 'send_next_buffer_to_destination'."
                                                                 "Actual was:%d", mock_called_send_next_buffer_to_destination);
    cr_assert (0 == mock_called_decode_http_message_header, "Should have been called 'decode_http_message_header'."
                                                            "Actual was:%d", mock_called_decode_http_message_header);
    cr_assert (0 == mock_called_http_message_receive_body, "Should have been called 'http_message_receive_body'."
                                                           "Actual was:%d", mock_called_http_message_receive_body);
    cr_assert (0 == mock_called_read_next_buffer_from_source, "Should have been called 'read_next_buffer_from_source'."
                                                              "Actual was:%d", mock_called_read_next_buffer_from_source);
    cr_assert (0 == mock_called_http_message_decode_response_type, "Should have been called 'http_message_decode_response_type'."
                                                                   "Actual was:%d", mock_called_http_message_decode_response_type);
    release_reply (reply);
}
