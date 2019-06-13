#include "../../src/http/http_connection.h"
#include "../../src/http/request_replies.h"
#include "../../src/http/http_request.h"
#include "../../src/route.h"
#include <criterion/criterion.h>
#include <stack.h>

Test (new_http_connection, result_should_never_be_null)
{
    ri_connection_t* c = new_http_connection(NULL);

    cr_assert_not_null (c, "Should not be null");
    cr_assert_not_null (c->requestReplies, "request_replies stsack should not be null");
}

Test (new_http_connection, request_replies_should_be_stack)
{
    ri_connection_t* c = new_http_connection(NULL);
    stack_head_t* st = c->requestReplies;
    cr_assert (0 == stack_depth(st), "Depth should be 0 after intialization, not:%d", stack_depth(st));
    stack_push (st, (void*)0xFFFFFFFFl);
    cr_assert(1 == stack_depth(st), "Depth should be 1 after 1 push, not:%d", stack_depth(st));
    cr_assert (0xFFFFFFFFl == (long)stack_pop(st), "Should retrieve the value with a pop");
}

extern request_replies_t* prepare_for_next_request_replies (ri_connection_t* conn);
extern int mock_called_new_request_replies;
extern long mock_param_1_new_request_replies[];
extern int mock_param_2_new_request_replies[];
extern long mock_param_3_new_request_replies[];
extern int mock_param_4_new_request_replies[];
extern request_replies_t* mock_result_new_request_replies;
Test (prepare_for_next_request_replies, should_pass_all_parameters_and_never_return_null)
{
    route_t* route = (route_t*) malloc (sizeof(route_t));
    route->in_connector = (in_connector_t*) malloc (sizeof(in_connector_t));
    route->out_connections = 44;
    route->out_connectors = (out_connector_t**) malloc (sizeof(out_connector_t*));
    route->forward_mode = 77;
    ri_connection_t* c = new_http_connection(route);
    mock_called_new_request_replies = 0;
    mock_result_new_request_replies = (request_replies_t*) malloc (sizeof(request_replies_t));
    request_replies_t* rr = prepare_for_next_request_replies (c);

    cr_assert_not_null(rr, "Returned request_replies should not be null");
    cr_assert (1 == mock_called_new_request_replies, "Should have called 'new_requets_replies."
                                                     "Actual call was:%d", mock_called_new_request_replies);
    cr_assert((long)route->in_connector == mock_param_1_new_request_replies[0], "Should have passed 'in_connector'"
                                                                          "as first parameter");
    cr_assert(route->out_connections == mock_param_2_new_request_replies[0], "Should have passed number of servers "
                                                                             "as second parameter");
    cr_assert((long)route->out_connectors == mock_param_3_new_request_replies[0], "should have passed ref to out connectors"
                                                                            "as third parameter");
    cr_assert(77 == mock_param_4_new_request_replies[0], "should have passed forward mode as fourth parameter");
    free (route->out_connectors);
    free(route->in_connector);
    free(route);
    free(c);
    free(rr);
}

extern int mock_called_accept_opening_request_from_client;
extern int mock_result_accept_opening_request_from_client;
extern long mock_param_1_accept_opening_request_from_client[];
Test(open_connection, should_forward_call_to_open_connection_and_return_result)
{
    ri_connection_t* c = new_http_connection(NULL);
    request_replies_t* rr = (request_replies_t*) malloc (sizeof(request_replies_t));
    stack_push(c->requestReplies, rr);
    mock_called_accept_opening_request_from_client = 0;
    mock_result_accept_opening_request_from_client = 88;

    int res = open_connection(c);

    cr_assert(88 == res, "Should have passed the result back, expected 8, received:%d", res);
    cr_assert(1 == mock_called_accept_opening_request_from_client, "Should have called 'accept_opening_request_from_client'. Actual "
                                                                   "was:%d", mock_called_accept_opening_request_from_client);
    cr_assert((long)rr == mock_param_1_accept_opening_request_from_client[0], "Should have passed request replies as param 1");
}

extern int mock_called_close_client_connection;
extern int mock_called_close_server_connection;
extern long mock_param_1_close_client_connection[];
Test (close_connection, zero_server_conection)
{
    ri_connection_t* c = new_http_connection(NULL);
    request_replies_t* rr = (request_replies_t*) malloc (sizeof(request_replies_t));
    rr->request = (request_t*) malloc (sizeof(request_t));
    rr->out_connections = 0;
    stack_push(c->requestReplies, rr);
    mock_called_close_client_connection = 0;
    mock_called_close_server_connection = 0;

    close_connection(c);

    cr_assert (1 == mock_called_close_client_connection, "Should have called 'close_client_connection."
                                                         "Actual was:%d", mock_called_close_client_connection);
    cr_assert((long)rr->request == mock_param_1_close_client_connection[0], "should have passed 'request' to "
                                                                   "'close_client_connection");
    cr_assert (0 == mock_called_close_server_connection, "Should not have called 'close_server_connection."
                                                         "Actual was:%d", mock_called_close_server_connection);
    free (rr->request);
    free (rr);
    free (c);
}

Test (close_connection, two_server_conection)
{
    ri_connection_t* c = new_http_connection(NULL);
    request_replies_t* rr = (request_replies_t*) malloc (sizeof(request_replies_t));
    rr->request = (request_t*) malloc (sizeof(request_t));
    rr->out_connections = 2;
    rr->replies = (reply_t**) malloc (sizeof(reply_t*) * 2);
    rr->replies[0] = (reply_t*) malloc (sizeof(reply_t));
    rr->replies[1] = (reply_t*) malloc (sizeof(reply_t));
    stack_push(c->requestReplies, rr);
    mock_called_close_client_connection = 0;
    mock_called_close_server_connection = 0;

    close_connection(c);

    cr_assert (1 == mock_called_close_client_connection, "Should have called 'close_client_connection."
                                                         "Actual was:%d", mock_called_close_client_connection);
    cr_assert((long)rr->request == mock_param_1_close_client_connection[0], "should have passed 'request' to "
                                                                            "'close_client_connection");
    cr_assert (2 == mock_called_close_server_connection, "Should not have called 'close_server_connection."
                                                         "Actual was:%d", mock_called_close_server_connection);
    cr_assert ((long)rr->replies[0], "Should have passed the first reply as parameter to server close function");
    cr_assert ((long)rr->replies[1], "Should have passed the second reply as parameter to the second call"
                                     "to the server close function");
    free (rr->request);
    free (rr->replies[0]);
    free (rr->replies[1]);
    free (rr);
    free (c);
}

extern void process_request_replies (ri_connection_t* conn, request_replies_t* rr);
extern int mock_called_strategy_sequential_request_replies;
extern long mock_param_1_strategy_sequential_request_replies[];
Test (process_request_replies, delegate_to_request_replies)
{
    ri_connection_t* c = new_http_connection(NULL);
    request_replies_t* rr = (request_replies_t*) malloc (sizeof(request_replies_t));
    rr->request = (request_t*) malloc (sizeof(request_t));
    rr->request->http_message = (http_message_t*) malloc (sizeof(http_message_t));
    rr->request->http_message->raw_message_length = 50;
    c->total_bytes = 5;
    c->total_messages = 12;
    mock_called_strategy_sequential_request_replies = 0;

    process_request_replies (c, rr);

    cr_assert(55 == c->total_bytes, "Should have updated total size to 55. Instead is:%lu", c->total_bytes);
    cr_assert(13 == c->total_messages, "Should have added 1 message. Instead is:%d", c->total_messages);
    cr_assert(1 == mock_called_strategy_sequential_request_replies, "Should have called 'strategy_sequential_request_replies'."
                                                                    "Actual was:%d", mock_called_strategy_sequential_request_replies);
    cr_assert((long)rr == mock_param_1_strategy_sequential_request_replies[0], "Should have passed request_replies in"
                                                                               "first parameter");
}

extern void process_connection (ri_connection_t* conn);
extern int mock_called_get_request_connection_keep_alive;
extern int mock_result_get_request_connection_keep_alive;
Test (process_connection, call_all_methods)
{
    route_t* route = (route_t*) malloc (sizeof(route_t));
    route->in_connector = (in_connector_t*) malloc (sizeof(in_connector_t));
    route->out_connections = 44;
    route->out_connectors = (out_connector_t**) malloc (sizeof(out_connector_t*));
    ri_connection_t* c = new_http_connection(route);
    request_replies_t* rr = (request_replies_t*) malloc (sizeof(request_replies_t));
    rr->request = (request_t*) malloc (sizeof(request_t));
    rr->request->http_message = (http_message_t*) malloc (sizeof(http_message_t));
    mock_called_strategy_sequential_request_replies = 0;
    mock_called_get_request_connection_keep_alive = 0;
    mock_result_new_request_replies = rr;
    mock_result_get_request_connection_keep_alive = 0;
    mock_called_new_request_replies = 0;

    process_connection (c);

    cr_assert(1 == mock_called_strategy_sequential_request_replies, "Should have called 'strategy_sequential_request_replies'."
                                                                    "Actual was:%d", mock_called_strategy_sequential_request_replies);
    cr_assert(1 == mock_called_get_request_connection_keep_alive, "Should have called 'get_request_connection_keep_alive'."
                                                                    "Actual was:%d", mock_called_get_request_connection_keep_alive);
    cr_assert(1 == mock_called_new_request_replies, "Should have called 'new_request_replies'."
                                                                  "Actual was:%d", mock_called_new_request_replies);

}

Test (run_session, no_connection)
{
    route_t* route = (route_t*) malloc (sizeof(route_t));
    route->in_connector = (in_connector_t*) malloc (sizeof(in_connector_t));
    route->out_connections = 44;
    route->out_connectors = (out_connector_t**) malloc (sizeof(out_connector_t*));
    ri_connection_t* c = new_http_connection(route);

    mock_called_new_request_replies = 0;
    mock_called_accept_opening_request_from_client = 0;
    mock_called_get_request_connection_keep_alive = 0;
    mock_result_accept_opening_request_from_client = -1;
    mock_called_close_client_connection = 0;

    run_session (c);

    cr_assert(0 == mock_called_new_request_replies, "Should have called 'new_request_replies'."
                                                    "Actual was:%d", mock_called_new_request_replies);
    cr_assert(1 == mock_called_accept_opening_request_from_client, "Should have called 'accept_opening_request_from_client'."
                                                    "Actual was:%d", mock_called_accept_opening_request_from_client);
    cr_assert(0 == mock_called_get_request_connection_keep_alive, "Should not have called 'get_request_connection_keep_alive'."
                                                                  "Actual was:%d", mock_called_get_request_connection_keep_alive);
    cr_assert(0 == mock_called_close_client_connection, "Should not have called 'close_client_connection'."
                                                                  "Actual was:%d", mock_called_close_client_connection);
}

Test (run_session, open_connection)
{
    route_t* route = (route_t*) malloc (sizeof(route_t));
    route->in_connector = (in_connector_t*) malloc (sizeof(in_connector_t));
    route->out_connections = 44;
    route->out_connectors = (out_connector_t**) malloc (sizeof(out_connector_t*));
    ri_connection_t* c = new_http_connection(route);

    mock_called_new_request_replies = 0;
    mock_called_accept_opening_request_from_client = 0;
    mock_called_get_request_connection_keep_alive = 0;
    mock_result_accept_opening_request_from_client = 1;
    mock_called_close_client_connection = 0;
    request_replies_t* rr = (request_replies_t*) malloc (sizeof(request_replies_t));
    rr->request = (request_t*) malloc (sizeof(request_t));
    rr->request->http_message = (http_message_t*) malloc (sizeof(http_message_t));
    mock_result_new_request_replies = rr;
    run_session (c);

    cr_assert(1 == mock_called_new_request_replies, "Should have called 'new_request_replies'."
                                                    "Actual was:%d", mock_called_new_request_replies);
    cr_assert(1 == mock_called_accept_opening_request_from_client, "Should have called 'accept_opening_request_from_client'."
                                                                   "Actual was:%d", mock_called_accept_opening_request_from_client);
    cr_assert(1 == mock_called_get_request_connection_keep_alive, "Should not have called 'get_request_connection_keep_alive'."
                                                                  "Actual was:%d", mock_called_get_request_connection_keep_alive);
    cr_assert(1 == mock_called_close_client_connection, "Should not have called 'close_client_connection'."
                                                        "Actual was:%d", mock_called_close_client_connection);
}