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
    int out_connections;
    pthread_t async_replies_thread;
} request_replies_t;

request_replies_t* new_request_replies (ri_in_connector_t* connector_def, int number_of_servers, ri_out_connector_t** server_conns);

/*! Waits for a new request from the client and returns only after the header has been fully decoded
 */
void accept_opening_request_from_client (request_replies_t* rr);

void strategy_sequential_request_replies (request_replies_t* rr);

void release_buffer_after_processing(request_replies_t* request);

void decode_request_message_header(request_t *request);

void forward_request_to_all_servers(request_replies_t *rr);

void wait_4_all_sender_to_complete(request_replies_t *request);

void close_in_and_out_connections (request_replies_t* rr);

void free_request_replies (request_replies_t* rr);

void *sync_request_reply_to_server(reply_t *reply);

#endif
