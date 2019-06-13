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
#include "request_replies.h"
#include <stack.h>

ri_connection_t* new_http_connection (route_t* route)
{
    ri_connection_t* res = (ri_connection_t *) malloc(sizeof(ri_connection_t));
    res->route = route;
    res->requestReplies = stack_init();
    return res;
}

request_replies_t* prepare_for_next_request_replies (ri_connection_t* conn)
{
    request_replies_t* rr = new_request_replies(conn->route->in_connector, conn->route->out_connections, conn->route->out_connectors, conn->route->forward_mode);
    return rr;
}

/*!
 * @param conn
 * @return <0 error
 */
int open_connection(ri_connection_t *conn)
{
    return accept_opening_request_from_client (stack_top(conn->requestReplies));
}

void close_connection (ri_connection_t *conn)
{
    request_replies_t* rr = stack_top (conn->requestReplies);
    close_client_connection (rr->request);
    for (int i=0; i < rr->out_connections; i++) {
        close_server_connection (rr->replies[i]);
    }
}

void process_request_replies (ri_connection_t* conn, request_replies_t* rr)
{
    conn->total_messages += 1;
    conn->total_bytes += rr->request->http_message->raw_message_length;
    strategy_sequential_request_replies (rr);
}

void process_connection (ri_connection_t* conn)
{
    int keep_alive;
    do {
        request_replies_t* rr = prepare_for_next_request_replies (conn);
        stack_push (conn->requestReplies, rr);
        process_request_replies (conn, rr);
        keep_alive = get_request_connection_keep_alive (rr->request);
    } while (keep_alive);
}

void* run_session (ri_connection_t* conn)
{

    int r = open_connection (conn);
    if (r > 0) {
        process_connection (conn);
        close_connection (conn);
    }
    return NULL;
}

void release_runtime_route (ri_connection_t* conn)
{
    stack_node_t* rr = NULL;
    while ((rr = stack_pop (conn->requestReplies)) != NULL ) {
        free_request_replies (rr);
    }
    free(conn->requestReplies);

    free(conn);
}