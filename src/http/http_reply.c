#include <stdlib.h>
#include "http_reply.h"
#include "../route_def.h"
#include "http_message.h"

/*! REFACTOR. Currently only accepts SOCKET as client source.
 *
 * @param conn
 * @param out_conn
 * @param forward_mode
 * @return
 */
reply_t *create_reply(const ri_connection_t *conn, ri_out_connector_t *out_conn, int forward_mode) {
    reply_t *reply = (reply_t *) malloc(sizeof(reply_t));
    reply->flow = out_conn->flow;
    reply->forward_mode = forward_mode;

    reply->client_transmission_type = TYPE_SOCKET;
    switch (reply->client_transmission_type) {
        case TYPE_SOCKET:
            reply->client.sock->fd = conn->fd;
            break;
    }

    reply->server_transmission_type = out_conn->type;
    switch (reply->server_transmission_type) {
        case TYPE_SOCKET:
            reply->server.sock = out_conn->content.sock;
            break;
        case TYPE_FILE:
            reply->server.file = out_conn->content.file;
            break;
    }
    reply->response_callback = out_conn->response_callback;
    return reply;
}

reply_t** new_replies_set(const ri_connection_t *conn)
{
    reply_t** replies_set = (reply_t **) malloc(sizeof(reply_t *) * conn->route->out_connections);
    for (int i = 0; i < conn->route->out_connections; i++) {
        replies_set[i] = create_reply(conn, conn->route->out_connectors[i], conn->route->forward_mode);
    }
    return replies_set;
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

