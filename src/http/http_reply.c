#include <stdlib.h>
#include <pthread.h>
#include "http_reply.h"
#include "../route_def.h"
#include "http_message.h"
#include "../buffers/shared_buffers.h"

/*! REFACTOR. Currently only accepts SOCKET as client source.
 *
 * @param conn
 * @param out_conn
 * @param forward_mode
 * @return
 */
reply_t *create_reply( int (*open_connection) (void* conn_params),
                       int (*send_data) (void* conn_params, char* dest_buffer, int size),
                       int (*receive_data) (void* conn_params, char* dest_buffer, int max_buffer_size),
                       int (*close_connection) (void* conn_params),
                       void* connection_params)
{
    reply_t* reply = (reply_t *) malloc(sizeof(reply_t));
    reply->open_connection = open_connection;
    reply->send_data = send_data;
    reply->receive_data = receive_data;
    reply->close_connection = close_connection;
    reply->connection_params = connection_params;

    return reply;
}

struct envelope {
    http_message_t* requestMessage;
    reply_t* reply;
    char last_server_in_list;
};

void* send_request (struct envelope* env)
{
    send_next_buffer_to_destination (env->reply->send_data, env->reply->connection_params, env->requestMessage, env->last_server_in_list);
    return NULL;
}

void connect_to_server (reply_t* reply)
{
    reply->open_connection (reply->connection_params);
}

/*! REFACTOR: should be agnostic of channel
 *
 * @param thread_data
 */
void reply_to_client(reply_t* reply) {
    printf("reply_to_client\n");
    send_next_buffer_to_destination (reply->send_data, reply->connection_params, reply->response_message, 1);

    printf("sent to client\n");
}

void accept_reply_from_server(reply_t *reply) {
    reply->response_message = receive_new_http_message(reply->server.sock->fd, reply->receive_data, TX_BUFFER_SIZE);
    reply->type = http_message_decode_response_type(reply->response_message);
}

void receive_reply(reply_t *reply) {
    accept_reply_from_server(reply);
    decode_http_message_header(reply->response_message);
    printf("Body length:%d\n", reply->response_message->body_length);
    http_message_receive_body(reply->response_message);
    printf("Body received\n");
}

void send_request_receive_reply (struct envelope* env)
{
    connect_to_server (env->reply);
    send_request(env);
    receive_reply (env->reply);
}

void send_request_to_server_and_wait_reply(http_message_t* msg, reply_t *reply, char last_server_in_list, char forward_mode) {
    struct envelope* env = (struct envelope*) malloc (sizeof(struct envelope));
    env->requestMessage = msg;
    env->last_server_in_list = last_server_in_list;
    if (forward_mode != FORWARD_MODE_SEQ) {
        int rc = pthread_create(&reply->pthread, NULL, (void* (*) (void*)) send_request_receive_reply, env);
    } else {
        send_request_receive_reply(env);
    }
    free (env);
}

void release_reply(reply_t *reply) {
    http_message_free(reply->response_message);
    free(reply);
}

