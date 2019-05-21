#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "route_def.h"
#include "frame_buffers/shared_buffers.h"
#include "connection.h"
#include "request_reply.h"
#include "socket/socket_connector.h"
#include "http/http_message.h"
#include "http/http_first_line.h"

void release_buffer_after_processing(request_t *request) {
    printf("Release_buffer_after_processing\n");
    int i;
    switch (request->forward_mode) {
        case FORWARD_MODE_SYNC:
            for (i = 0; i < request->out_connections; i++) {
                pthread_join(request->replies[i]->pthread, NULL);
            }
            //No break here
        case FORWARD_MODE_SEQ:
            buffer_has_been_sent(request->http_message);
            //close_in_connector(re);
            break;
        case FORWARD_MODE_ASYNC:
            wait_4_all_sender_to_complete(request);
            break;
    }
    printf("release_buffer_after_processing:END\n");
}

void *push_data_2_destination(void *params) {
    reply_t *reply = (reply_t *) params;
    //printf("BUFFER:%s\n", reply->request->http_message->buffers[reply->request->http_message->last_sent]);
    switch (reply->type) {
        case TYPE_SOCKET:
            reply->content.sock->consumer_callback(reply);
            break;
        case TYPE_FILE:
            printf("callback is:%s\n", (reply->content.file->file == NULL) ? ("NULL") : ("NOT NULL"));
            reply->content.file->output_callback(reply);
            break;
        default:
            printf("Argl\n");
    }
    return NULL;
}

reply_t *create_reply(const ri_connection_t *conn, ri_out_connector_t *out_conn) {
    reply_t *reply = (reply_t *) malloc(sizeof(reply_t));
    reply->flow = out_conn->flow;
    reply->type = out_conn->type;
    switch (reply->type) {
        case TYPE_SOCKET:
            reply->content.sock = out_conn->content.sock;
            break;
        case TYPE_FILE:
            reply->content.file = out_conn->content.file;
            break;
    }
    reply->response_callback = out_conn->response_callback;
    return reply;
}

request_t* request_init(const ri_connection_t *conn) {
    request_t *request = (request_t *) malloc(sizeof(request_t));
    request->forward_mode = conn->route->forward_mode;
    request->buffer_size = TX_BUFFER_SIZE;
    request->http_message = NULL;
    request->out_connections = conn->route->out_connections;
    request->in_response.sock_fd = conn->fd;
    request->replies = (reply_t **) malloc(sizeof(reply_t *) * conn->route->out_connections);
    for (int i = 0; i < conn->route->out_connections; i++) {
        request->replies[i] = create_reply(conn, conn->route->out_connectors[i]);
        request->replies[i]->request = request;
    }
    return request;
}

request_t* receive_new_request (const ri_connection_t *conn)
{
    request_t* request = request_init(conn);
    request->http_message = receive_new_http_message(conn->fd);
    http_message_decode_request_type (request->http_message);
    read_next_buffer_from_source(request->http_message);
    decode_http_message_header(conn->fd, request->http_message);
}

void accept_reply_from_server(reply_t *reply) {
    reply->response_message = receive_new_http_message(reply->content.sock->fd);
    http_message_decode_response_type(reply->response_message);
}

void receive_reply(reply_t *reply) {
    accept_reply_from_server(reply);
    int start_of_body = decode_http_message_header(reply->content.sock->fd, reply->response_message);
    printf("Body length:%d\n", reply->response_message->body_length);
    receive_body(reply->content.sock->fd, reply->response_message, start_of_body);
    printf("Body received\n");
}

request_t *accept_opening_request_from_client(const ri_connection_t *conn) {
    printf("accept_opening_request_from_client\n");
    request_t* request = receive_new_request(conn);;

    if (request != NULL) {
        switch (request->http_message->function) {
            case HTTP_REQUEST_GET:
                break;
            case HTTP_REQUEST_POST:
                printf("Not implemented yet\n");
                exit(0);
        }
        //printf("%s\n", get_buffer(request->http_message->buffers[request->http_message->last_received]));
    }
    return request;
}

void reply_to_client(void *thread_data) {
    printf("reply_to_client\n");
    reply_t *reply = (reply_t *) thread_data;
    send_next_buffer_to_destination (reply->response_message, 1, reply->request->in_response.sock_fd);

    printf("sent to client\n");
}

void release_reply(reply_t *reply) {
    http_message_free(reply->response_message);
    free(reply);
}

void release_request(request_t *d) {
    http_message_free(d->http_message);
    for (int i = 0; i < d->out_connections; i++) {
        release_reply(d->replies[i]);
    }
    free(d);
}

void *async_join_threads(void *params) {
    request_t *request = (request_t *) params;
    printf("Joining results...\n");
    int i = 0;
    for (int i = 0; i < request->out_connections; i++) {
        printf("Join thread at:%lu\n", request->replies[i]->pthread);
        pthread_join(request->replies[i]->pthread, NULL);
        //release_conn_runtime_thread_data(at->route->out_connectors[i]->data[at->dest_threads[i]    ]);
    }
    printf("...joined\n");
    release_request(request);
    buffer_has_been_sent(request->http_message);
    //close(at->route->in_connector->content.sock->fd);
    return NULL;
}

void wait_4_all_sender_to_complete(request_t *request) {
    int rc = pthread_create(&request->async_replies_thread, NULL, async_join_threads, request);
}

void forward_to_one_server(reply_t *reply) {
    if (reply->request->forward_mode != FORWARD_MODE_SEQ) {
        int rc = pthread_create(&reply->pthread, NULL, push_data_2_destination, reply);
    } else {
        push_data_2_destination(reply);
    }
}

void forward_message_to_all_servers(request_t *request) {
    for (int i = 0; i < request->out_connections; i++) {
        forward_to_one_server(request->replies[i]);
    }
}

void decode_request_message_header(request_t *request) {
    decode_http_message_header(request->in_response.sock_fd, request->http_message);
}

void process_request_message_body(request_t *request) {
    switch (request->http_message->function) {
        case HTTP_REQUEST_GET:
            break;
        case HTTP_REQUEST_POST:
            break;
    }
    forward_message_to_all_servers(request);
}

//TODO REFACTOR: the following 3 methods are symetric: same functionality, one foe socket, the other one for file. Reafctor to make generic
// at this level and push down what is specific
void *sync_request_reply_to_server(reply_t *reply) {
    int socket = connect_to_server(reply->content.sock->server_name, reply->content.sock->port);
    reply->content.sock->fd = socket;
    send_next_buffer_to_destination (reply->request->http_message, 1, socket);
    printf("$$$Message sent:\n");
    receive_reply(reply);
    reply->response_callback(reply);
}
//char resp[] = "HTTP/1.1 200 OK\nDate: Mon, 23 May 2005 22:38:34 GMT\nContent-Type: text/html; charset=UTF-8\nContent-Length: 138\nLast-Modified: Wed, 08 Jan 2003 23:11:55 GMT\nServer: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\nETag: \"3f80f-1b6-3e1cb03b\"\nAccept-Ranges: bytes\n\n<html>\n<head>\n <title>An Example Page</title>\n</head>\n<body>\n Hello World, this is a very simple HTML document.\n</body>\n</html>";
char resp[] = "HTTP/1.1 200 OK\nDate: Mon, 23 May 2005 22:38:34 GMT\nContent-Type: text/html; charset=UTF-8\nContent-Length: 14\nLast-Modified: Wed, 08 Jan 2003 23:11:55 GMT\nServer: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\nETag: \"3f80f-1b6-3e1cb03b\"\nAccept-Ranges: bytes\n\n<html>\n</html>";

reply_t* create_response(reply_t* data){
    int buff_no = alloc_buffer();
    char* buffer = get_buffer(buff_no);
    http_message_t* msg = http_message_init(data->content.file->file);
    data->response_message = msg;
    // TODO: must be refactoredstrcpy(msg->buffer, resp);
    msg->raw_message_length = strlen(resp);
    return data;
}

void file_writer(void *params)
{
    reply_t* reply = (reply_t*) params;
    // TODO must be refactored fputs(reply->request->http_message->buffer, reply->content.file->file);
    fflush(reply->content.file->file);
    if(reply->flow == FLOW_BIDIRECTIONAL){
        reply->response_callback(create_response(reply));
    }
}


