#ifndef HTTP_REPLY_H
#define HTTP_REPLY_H

#include <pthread.h>
#include "http_message.h"

typedef struct {
    int type;       //200, 40x...
    int forward_mode;
    pthread_t pthread;

    void* connection_params; // ri_sock_connector_t, ri_file_connector_t
    int (*open_connection) (void* conn_params);
    int (*send_data) (void* conn_params, char* dest_buffer, int max_buffer_size);
    int (*receive_data) (void* conn_params, char* dest_buffer, int max_buffer_size);
    int (*close_connection) (void* conn_params);
    http_message_t *response_message;
} reply_t;



reply_t* create_reply( int (*open_connection) (void* conn_params),
                    int (*send_data) (void* conn_params, char* dest_buffer, int max_buffer_size),
                    int (*receive_data) (void* conn_params, char* dest_buffer, int max_buffer_size),
                    int (*close_connection) (void* conn_params),
                    void* connection_params);

void connect_to_server (reply_t* reply);

void send_request_to_server_and_wait_reply(http_message_t* message, reply_t *reply, char last_server_in_list, char forward_mode);

void reply_to_client(reply_t* reply);

void close_server_connection (reply_t* reply);

/*! RElease resources attached
 * \private
 * @param reply
 */
void release_reply(reply_t *reply);

#endif //HTTP_REPLY_H