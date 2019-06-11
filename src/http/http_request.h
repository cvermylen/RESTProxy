#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "http_message.h"

typedef struct request {
    int type;       //GET, POST...
    int buffer_size;
    http_message_t *http_message;
    void* connection_params; // ri_sock_connector_t, ri_file_connector_t
    int (*open_connection) (void* conn_params);
    int (*send_data) (void* conn_params, char* dest_buffer, int max_buffer_size);
    int (*close_connection) (void* conn_params);
} request_t;

/*! \private
 *
 * @return new malloc'd
 */
request_t* new_http_request (int(*open_connection) (void* conn_params),
        int(*feeder)(),
        int (*send_data) (void* conn_params, char* dest_buffer, int max_buffer_size),
        int(*close_connection) (void* conn_params), void* connection_params);

void receive_new_request_from_client(request_t *request);

void process_request_message_body (request_t* request);

int get_request_connection_keep_alive (request_t* request);

void close_client_connection (request_t* request);

void decode_request_message_header (request_t *request);

/*! RElease resources attached
 * \private
 * @param request
 */
request_t* release_request(request_t *request);

#endif //HTTP_REQUEST_H
