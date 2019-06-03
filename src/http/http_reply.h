#ifndef GATEWAY_HTTP_REPLY_H
#define GATEWAY_HTTP_REPLY_H

typedef struct {
    int type;       //200, 40x...
    int forward_mode;
    request_t *request;
    pthread_t pthread;
    int flow;
    int server_transmission_type;  //TYPE_SOCKET, TYPE_FILE
    union {
        ri_sock_connector_t *sock;
        ri_file_connector_t *file;
        ri_workflow_connector_t *workflow;
    } server;
    http_message_t *response_message;

    void (*response_callback)(void *ri_response);
} reply_t;

#endif //GATEWAY_HTTP_REPLY_H

reply_t** new_replies_set(const ri_connection_t *conn);

void receive_reply(reply_t *reply, int (*feeder)(int fd, char* buffer, int buffer_size));

/*! RElease resources attached
 * \private
 * @param reply
 */
void release_reply(reply_t *reply);

