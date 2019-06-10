#include "../../src/http/http_reply.h"
#include <stdlib.h>

int mock_called_create_reply;
reply_t *create_reply( int (*open_connection) (void* conn_params),
                       int (*send_data) (void* conn_params, char* dest_buffer, int size),
                       int (*receive_data) (void* conn_params, char* dest_buffer, int max_buffer_size),
                       int (*close_connection) (void* conn_params),
                       void* connection_params) {
    mock_called_create_reply += 1;
    reply_t* reply = (reply_t*) malloc (sizeof(reply_t));
    return reply;
}

struct envelope {
    http_message_t* requestMessage;
    reply_t* reply;
    char last_server_in_list;
};

int mock_called_send_request;
void* mock_result_send_request;
void* send_request (struct envelope* env) {
    mock_called_send_request += 1;
    return mock_result_send_request;
}

int mock_called_connect_to_server;
void connect_to_server (reply_t* reply)
{
    mock_called_connect_to_server += 1;
}

int mock_called_reply_to_client;
void reply_to_client(reply_t* reply) {
    mock_called_reply_to_client += 1;
}

int mock_called_accept_reply_from_server;
void accept_reply_from_server(reply_t *reply) {
    mock_called_accept_reply_from_server += 1;
}

int mock_called_receive_reply;
void receive_reply(reply_t *reply) {
    mock_called_receive_reply += 1;
}

int mock_called_send_request_receive_reply;
void send_request_receive_reply (struct envelope* env)
{
    mock_called_send_request_receive_reply += 1;
}

int mock_called_send_request_to_server_and_wait_reply;
void send_request_to_server_and_wait_reply(http_message_t* msg, reply_t *reply, char last_server_in_list, char forward_mode) {
    mock_called_send_request_to_server_and_wait_reply += 1;
}

int mock_called_close_server_connection;
void close_server_connection (reply_t* reply)
{
    mock_called_close_server_connection += 1;
}

int mock_called_release_reply;
reply_t* mock_result_release_reply;
reply_t* release_reply(reply_t *reply) {
    mock_called_release_reply += 1;
    return mock_result_release_reply;
}

