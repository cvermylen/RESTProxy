#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>

#include "../route_def.h"
#include "../buffers/shared_buffers.h"
#include "http_connection.h"
#include "../socket/socket_connector.h"
#include "http_first_line.h"
#include "../route_instance.h"

#include <stack.h>

//stack_head_t *connections_stack;

ri_connection_t* new_http_connection (ri_route_t* route)
{
    ri_connection_t* res = (ri_connection_t *) malloc(sizeof(ri_connection_t));
    res->route = route;
    res->requestReplies = stack_init();
    return res;
}

request_replies_t* prepare_for_next_request_replies (ri_connection_t* conn)
{
    request_replies_t* rr = new_request_replies(conn->route->in_connector, conn->route->out_connections, conn->route->out_connectors);
    return rr;
}
/*!
 * @param conn
 * @return <0 error
 */
int open_connection(ri_connection_t *conn)
{
    accept_opening_request_from_client (stack_top(conn->requestReplies));
    return 0;
}

void close_connection (request_replies_t* rr)
{
    printf("Close connection:\n");
    close_client_connection (rr->request);
    for (int i=0; i < rr->out_connections; i++) {
        close_server_connection (rr->replies[i]);
    }
}

void process_request_replies (ri_connection_t* conn, request_replies_t* rr)
{
    printf("Received:%d\n", rr->request->http_message->raw_message_length);
    conn->total_messages += 1;
    conn->total_bytes += rr->request->http_message->raw_message_length;
    strategy_sequential_request_replies (rr);
}
/*! new version of 'receive_and_process_data_from_client'
 * @param conn
 */
void run_session (ri_connection_t* conn)
{
    request_replies_t* rr = prepare_for_next_request_replies (conn);
    int r = open_connection (conn);
    if (r > 0) {
        int keep_alive = 1;
        do {
            stack_push (conn->requestReplies, rr);
            process_request_replies (conn, rr);
            keep_alive = get_request_connection_keep_alive (rr->request);
            rr = prepare_for_next_request_replies (conn);
        } while (keep_alive);
    }
    close_connection (rr);
}

//REFACTOR should be removed
void *receive_and_process_data_from_client(void *params) {
    ri_connection_t *conn = (ri_connection_t *) params;
    ri_route_t *route = conn->route;
    conn->total_messages = 0;
    conn->total_bytes = 0;
    request_replies_t* rr = new_request_replies(route->in_connector, route->out_connections, route->out_connectors);
    do {
        accept_opening_request_from_client(rr);
        if (rr->request != NULL) {
            printf("Received:%d\n", rr->request->http_message->raw_message_length);
            conn->total_messages += 1;
            conn->total_bytes += rr->request->http_message->raw_message_length;
            decode_request_message_header(rr->request);
            process_request_message_body(rr->request);
            forward_request_to_all_servers(rr);
            release_buffer_after_processing(rr);
        } else {
            conn->error = errno;
        }
        printf("Next loop in accepting connection\n");
    } while (rr->request != NULL && rr->request->http_message->raw_message_length > 0);
    printf("##### End of connection\n");
    close_connection(conn);
    if (rr->request != NULL) release_request(rr->request);
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

