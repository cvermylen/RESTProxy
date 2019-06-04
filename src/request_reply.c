#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "route_def.h"
#include "buffers/shared_buffers.h"
#include "http/http_connection.h"
#include "request_reply.h"
#include "socket/socket_connector.h"
#include "http/http_message.h"
#include "http/http_first_line.h"
#include "buffers/circular_buffer.h"
#include "http/http_reply.h"

request_replies_t* new_request_replies (ri_in_connector_t* connector_def, int number_of_servers, ri_out_connector_t** server_conns)
{
    request_replies_t* rr = (request_replies_t*) malloc (sizeof(request_replies_t));
    rr->request = new_http_request((void*)connector_def->open_connection, connector_def->feed_data, connector_def->send_data, connector_def->close_connection, connector_def->connection_params);
    rr->replies = (reply_t **) malloc(sizeof(reply_t *) * number_of_servers);
    for (int i = 0; i < number_of_servers; i++) {
        rr->replies[i] = create_reply(server_conns[i]->open_connection, server_conns[i]->send_data, server_conns[i]->receive_data, server_conns[i]->close_connection, server_conns[i]->connection_params);
    }
    return rr;
}

void synchronize_all_senders (request_replies_t* rr)
{
    int i;
    switch (rr->forward_mode) {
        case FORWARD_MODE_SYNC:
            for (i = 0; i < rr->out_connections; i++) {
                pthread_join(rr->replies[i]->pthread, NULL);
            }
            //No break here
        case FORWARD_MODE_SEQ:
            break;
        case FORWARD_MODE_ASYNC:
            wait_4_all_sender_to_complete(rr);
            break;
    }
}

void release_buffer_after_processing(request_replies_t* rr) {
    printf("Release_buffer_after_processing\n");
    release_request(rr->request);
    for (int i = 0; i < rr->out_connections; i++) {
        release_reply(rr->replies[i]);
    }
    printf("release_buffer_after_processing:END\n");
}

void accept_opening_request_from_client (request_replies_t* rr)
{
    printf("accept_opening_request_from_client\n");

    receive_new_request_from_client(rr->request);
}

void *async_join_threads(void *params) {
    request_replies_t* request = (request_replies_t *) params;
    printf("Joining results...\n");
    int i = 0;
    for (int i = 0; i < request->out_connections; i++) {
        printf("Join thread at:%lu\n", request->replies[i]->pthread);
        pthread_join(request->replies[i]->pthread, NULL);
        //release_conn_runtime_thread_data(at->route->out_connectors[i]->data[at->dest_threads[i]    ]);
    }
    printf("...joined\n");

    //close(at->route->in_connector->content.sock->fd);
    return NULL;
}

void wait_4_all_sender_to_complete(request_replies_t *request) {
    int rc = pthread_create(&request->async_replies_thread, NULL, async_join_threads, request);
}

void forward_request_to_all_servers(request_replies_t *rr) {
    for (int i = 0; i < rr->out_connections; i++) {
        connect_to_server (rr->replies[i]);
        send_request_to_server(rr->request->http_message, rr->replies[i], (i == rr->out_connections -1), rr->forward_mode);
    }
}

void decode_request_message_header(request_t *request) {
    decode_http_message_header(request->http_message);
}

void strategy_sequential_request_replies (request_replies_t* rr)
{
    decode_request_message_header(rr->request);
    process_request_message_body(rr->request);
    forward_request_to_all_servers(rr);
    synchronize_all_senders (rr);
    release_buffer_after_processing(rr);
}

//TODO REFACTOR: the following 3 methods are symetric: same functionality, one foe socket, the other one for file. Reafctor to make generic
// at this level and push down what is specific
void *sync_request_reply_to_server(reply_t *reply) {
    int socket = socket_connect(reply->server.sock->server_name, reply->server.sock->port);
    reply->server.sock->fd = socket;
    send_next_buffer_to_destination (reply->request->http_message, 1, socket);
    printf("$$$Message sent:\n");
    receive_reply(reply, read_from_socket);
    reply->response_callback(reply);
}
//cha
// r resp[] = "HTTP/1.1 200 OK\nDate: Mon, 23 May 2005 22:38:34 GMT\nContent-Type: text/html; charset=UTF-8\nContent-Length: 138\nLast-Modified: Wed, 08 Jan 2003 23:11:55 GMT\nServer: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\nETag: \"3f80f-1b6-3e1cb03b\"\nAccept-Ranges: bytes\n\n<html>\n<head>\n <title>An Example Page</title>\n</head>\n<body>\n Hello World, this is a very simple HTML document.\n</body>\n</html>";
char resp[] = "HTTP/1.1 200 OK\nDate: Mon, 23 May 2005 22:38:34 GMT\nContent-Type: text/html; charset=UTF-8\nContent-Length: 14\nLast-Modified: Wed, 08 Jan 2003 23:11:55 GMT\nServer: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\nETag: \"3f80f-1b6-3e1cb03b\"\nAccept-Ranges: bytes\n\n<html>\n</html>";

reply_t* create_response(reply_t* data){
    int buff_no = alloc_buffer();
    char* buffer = get_buffer(buff_no);
    http_message_t* msg = new_http_message(0, NULL, 0);  //TODO parameters 2 & 3 are wrong
    data->response_message = msg;
    // TODO: must be refactoredstrcpy(msg->buffer, resp);
    msg->raw_message_length = strlen(resp);
    return data;
}

void file_writer(void *params)
{
    reply_t* reply = (reply_t*) params;
    // TODO must be refactored fputs(reply->request->http_message->buffer, reply->content.file->file);
    fflush(reply->server.file->file);
    if(reply->flow == FLOW_BIDIRECTIONAL){
        reply->response_callback(create_response(reply));
    }
}


