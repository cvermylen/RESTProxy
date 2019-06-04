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
    http_message_t* msg;
    int (*send_data) (void* conn_params, char* dest_buffer, int size);
    void* conn_params;
    char last_server_in_list;
};

void* send_request (struct envelope* env)
{
    send_next_buffer_to_destination (env->send_data, env->conn_params, env->msg, env->last_server_in_list);
    return NULL;
}

void send_request_to_server(http_message_t* msg, reply_t *reply, char last_server_in_list, char forward_mode) {
    struct envelope* env = (struct envelope*) malloc (sizeof(struct envelope));
    env->msg = msg;
    env->send_data = reply->send_data;
    env->conn_params = reply->connection_params;
    env->last_server_in_list = last_server_in_list;
    if (forward_mode != FORWARD_MODE_SEQ) {
        int rc = pthread_create(&reply->pthread, NULL, (void* (*) (void*)) send_request, env);
    } else {
        send_request(env);
    }
    free (env);
}

void connect_to_server (reply_t* reply)
{
    reply->open_connection (reply->connection_params);
}

/*! REFACTOR: should be agnostic of channel
 *
 * @param thread_data
 */
void reply_to_client(void *thread_data) {
    printf("reply_to_client\n");
    reply_t *reply = (reply_t *) thread_data;
    send_next_buffer_to_destination (reply->response_message, 1, reply->client.sock->fd);

    printf("sent to client\n");
}

void accept_reply_from_server(reply_t *reply, int (*feeder)(int fd, char* buffer, int buffer_size)) {
    reply->response_message = receive_new_http_message(reply->server.sock->fd, feeder, TX_BUFFER_SIZE);
    reply->type = http_message_decode_response_type(reply->response_message);
}

void receive_reply(reply_t *reply, int (*feeder)(int fd, char* buffer, int buffer_size)) {
    accept_reply_from_server(reply, feeder);
    decode_http_message_header(reply->response_message);
    printf("Body length:%d\n", reply->response_message->body_length);
    http_message_receive_body(reply->response_message);
    printf("Body received\n");
}

void release_reply(reply_t *reply) {
    http_message_free(reply->response_message);
    free(reply);
}

