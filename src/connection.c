#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "route_def.h"
#include "shared_buffers.h"
#include "connection.h"
#include "accept_connections.h"
#include "socket_connector.h"
#include "request_reply.h"
#include "http/http_first_line.h"
#include <stack.h>

stack_head_t *connections_stack;
int program_should_continue = 1;

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
            free_buffer(request->http_message->buffer_no);
            //close_in_connector(re);
            break;
        case FORWARD_MODE_ASYNC:
            wait_4_all_sender_to_complete(request);
            break;
    }
    printf("release_buffer_after_processing:END\n");
}

void *receive_and_process_data_from_client(void *params) {
    ri_connection_t *conn = (ri_connection_t *) params;
    ri_route_t *route = conn->route;
    conn->total_messages = 0;
    conn->total_bytes = 0;
    request_t *request = NULL;
    do {
        request = accept_opening_request_from_client(conn);
        if (request != NULL) {
            printf("Received:%d\n", request->http_message->raw_message_length);
            conn->total_messages += 1;
            conn->total_bytes += request->http_message->raw_message_length;
            decode_request_message_header(request);
            process_request_message_body(request);
            release_buffer_after_processing(request);
        } else {
            conn->error = errno;
        }
        printf("Next loop in accepting connection\n");
    } while (request != NULL && request->http_message->raw_message_length > 0);
    printf("##### End of connection\n");
    close_connection(conn);
    if (request != NULL) release_request(request);
    pthread_exit(conn);
}

void *socket_connector(void *param) {
    ri_route_t *route = (ri_route_t *) param;
    connections_stack = stack_init();
    printf("socket_connector\n");
    pthread_t thread;
    do {
        ri_connection_t *cli = wait_4_connection_request(route);
        if (cli->fd > 0) {
            stack_push(connections_stack, cli);
            printf("%d\n", cli->fd);
            int rc = pthread_create(&thread, NULL, receive_and_process_data_from_client, (void *) cli);
        }
    } while (program_should_continue);
    printf("END SOCKET CONNECTOR\n");
    return NULL;
}

void close_remaining_connections() {
    printf("close_remaining_connections\n");
    ri_connection_t *conn = NULL;
    while ((conn = (ri_connection_t *) stack_pop(connections_stack))) {
        printf("Close this one:%d\n", conn->fd);
        close(conn->fd);
    }
    printf("Done closing\n");
}

void close_in_connector(ri_in_connector_t *conn) {
    close_remaining_connections();
    switch (conn->type) {
        case TYPE_SOCKET:
            printf("Close in connector socket:%d\n", conn->content.sock->fd);
            close(conn->content.sock->fd);
            break;
    }
    printf("End closing IN\n");
}

void close_connections(ri_route_t *route) {
    printf("Close connection\n");
    close_in_connector(route->in_connector);
    program_should_continue = 0;
}
