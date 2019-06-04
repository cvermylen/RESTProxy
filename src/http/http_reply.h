#ifndef GATEWAY_HTTP_REPLY_H
#define GATEWAY_HTTP_REPLY_H

#include "http_message.h"

typedef struct {
    int type;       //200, 40x...
    int forward_mode;
    request_t *request;
    pthread_t pthread;

    void* connection_params; // ri_sock_connector_t, ri_file_connector_t
    int (*open_connection) (void* conn_params);
    int (*send_data) (void* conn_params, char* dest_buffer, int max_buffer_size);
    int (*receive_data) (void* conn_params, char* dest_buffer, int max_buffer_size);
    int (*close_connection) (void* conn_params);
    //REFACTOR: remove
    int server_transmission_type;  //TYPE_SOCKET, TYPE_FILE
    union {
        ri_sock_connector_t *sock;
        ri_file_connector_t *file;
        ri_workflow_connector_t *workflow;
    } server;
    http_message_t *response_message;
    //REFACTOR: remove
    void (*response_callback)(void *ri_response);
} reply_t;

#endif //GATEWAY_HTTP_REPLY_H

reply_t* create_reply( int (*open_connection) (void* conn_params),
                    int (*send_data) (void* conn_params, char* dest_buffer, int max_buffer_size),
                    int (*receive_data) (void* conn_params, char* dest_buffer, int max_buffer_size),
                    int (*close_connection) (void* conn_params),
                    void* connection_params);

void connect_to_server (reply_t* reply);

void send_request_to_server(http_message_t* message, reply_t *reply, char last_server_in_list, char forward_mode);

/*! RElease resources attached
 * \private
 * @param reply
 */
void release_reply(reply_t *reply);

