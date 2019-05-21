#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>

#include "route_def.h"
#include "buffers/shared_buffers.h"
#include "connection.h"
#include "socket/socket_connector.h"
#include "request_reply.h"
#include "http/http_first_line.h"
#include "route_instance.h"

//#include <stack.h>

//stack_head_t *connections_stack;

ri_connection_t *wait_4_connection_request(ri_route_t *route) {
    ri_connection_t *res = (ri_connection_t *) malloc(sizeof(ri_connection_t));
    res->route = route;
    ri_sock_connector_t *conn = route->in_connector->content.sock;
    printf("BEFORE ACCEPTING new connection:%d\n", conn->fd);
    res->fd = accept(conn->fd, &(conn->cli_addr), &(conn->sockaddr_size));
    if (res->fd < 0) { printf("EXIT\n"); }
    printf("AFTER ACCEPTED new connection:%d\n", res->fd);
    return res;
}

void close_connection(ri_connection_t *conn) {
    printf("Close connection:%d\n", conn->fd);
    close(conn->fd);
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


void close_remaining_connections() {
    printf("close_remaining_connections\n");
    ri_connection_t *conn = NULL;
/*    while ((conn = (ri_connection_t *) stack_pop(connections_stack))) {
        printf("Close this one:%d\n", conn->fd);
        close(conn->fd);
    }*/
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

