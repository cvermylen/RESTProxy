#ifndef REQUEST_REPLY_H
#define REQUEST_REPLY_H

#include "http/http_connection.h"
#include "route_instance.h"
#include "http/http_request.h"
#include "http/http_reply.h"
#include "http/http_message.h"
#include "http/http_connection.h"

typedef struct {
    request_t* request;
    reply_t** replies;
    int forward_mode;
    union {
        int sock_fd;
        FILE *file_fd;
    } in_response;
    int out_connections;
    pthread_t async_replies_thread;
} request_replies_t;

request_replies_t* new_request_replies ((*client_feeder)(), void* client_connection_params);

/*! Waits for a new request from the client and returns only after the header has been fully decoded
 */
void accept_opening_request_from_client (request_replies_t* rr);

//request_t *create_request(const ri_connection_t *conn, int buff_no, char *buffer, int code, int sz);

void strategy_sequential_request_replies (request_replies_t* rr);

/*!
  \brief Data that an instance of an out connector needs to send out. The connector instance may be hold in a separate thread.
 */
request_t *
create_runtime_thread_data(int fd, ri_in_connector_t *in, ri_out_connector_t *conn, int buffer_size, char *buffer,
                           int data_len);

void release_buffer_after_processing(request_replies_t* request);

void decode_request_message(request_t *request);

void decode_request_message_header(request_t *request);

void release_conn_runtime_thread_data(request_t *d);

void reply_to_client(void *thread_data);

void forward_message_to_all_servers(request_replies_t* rr);

void *push_data_2_destination(void *params);

void wait_4_all_sender_to_complete(request_replies_t *request);

void *sync_request_reply_to_server(reply_t *reply);

#endif
