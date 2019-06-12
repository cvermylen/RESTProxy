#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../route_def.h"
#include "request_replies.h"
#include "../thread/pthread_wrap.h"

request_replies_t* new_request_replies (in_connector_t* connector_def, int number_of_servers, out_connector_t** server_conns, int forward_mode)
{
    request_replies_t* rr = (request_replies_t*) malloc (sizeof(request_replies_t));
    rr->request = new_http_request((void*)connector_def->open_connection, connector_def->feed_data, connector_def->send_data, connector_def->close_connection, connector_def->connection_params);
    rr->out_connections = number_of_servers;
    rr->replies = (reply_t **) malloc(sizeof(reply_t *) * number_of_servers);
    for (int i = 0; i < number_of_servers; i++) {
        rr->replies[i] = create_reply(server_conns[i]->open_connection, server_conns[i]->send_data, server_conns[i]->receive_data, server_conns[i]->close_connection, server_conns[i]->connection_params);
    }
    rr->forward_mode = forward_mode;
    return rr;
}

void join_client_threads (request_replies_t* request)
{
    printf("Joining results...\n");
    for (int i = 0; i < request->out_connections; i++) {
        printf("Join thread at:%i\n", (int)request->replies[i]->pthread);
        pthread_join_wrapper (request->replies[i]->pthread, NULL);
    }
    printf("...joined\n");
}

int wait_4_all_client_send_receive_to_complete(request_replies_t *rr)
{
    return pthread_create_wrapper (&rr->async_replies_thread, NULL, (void* (*) (void*))join_client_threads, rr);
}

void synchronize_all_senders (request_replies_t* rr)
{
    int i;
    switch (rr->forward_mode) {
        case FORWARD_MODE_SYNC:
            for (i = 0; i < rr->out_connections; i++) {
                pthread_join_wrapper (rr->replies[i]->pthread, NULL);
            }
            //No break here
        case FORWARD_MODE_SEQ:
            break;
        case FORWARD_MODE_ASYNC:
            wait_4_all_client_send_receive_to_complete(rr);
            break;
    }
}

void release_buffer_after_processing (request_replies_t* rr)
{
    rr->request = release_request (rr->request);
    for (int i = 0; i < rr->out_connections; i++) {
        release_reply (rr->replies[i]);
    }
}

int accept_opening_request_from_client (request_replies_t* rr)
{
    return receive_new_request_from_client (rr->request);
}

void forward_request_to_all_servers (request_replies_t *rr)
{
    for (int i = 0; i < rr->out_connections; i++) {
        send_request_to_server_and_wait_reply (rr->request->http_message, rr->replies[i], (i == rr->out_connections -1), rr->forward_mode);
    }
}

void forward_replies (request_replies_t* rr)
{
    for (int i= 0; i < rr->out_connections; i++) {
        reply_to_client (rr->replies[i]);
    }
}

void strategy_sequential_request_replies (request_replies_t* rr)
{
    decode_request_message_header(rr->request);
    process_request_message_body(rr->request);
    forward_request_to_all_servers(rr);
    synchronize_all_senders (rr);
    forward_replies (rr);
    release_buffer_after_processing(rr);
}

request_replies_t* free_request_replies (request_replies_t* rr)
{
    rr->request = release_request (rr->request);
    for (int i = 0; i < rr->out_connections; i++) {
        release_reply (rr->replies[i]);
    }
    free (rr);
    return NULL;
}