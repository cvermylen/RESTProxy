#include "../../src/http/request_replies.h"
#include "../../src/route.h"
#include "../../src/route_def.h"
#include <criterion/criterion.h>

extern int mock_called_new_http_request;
extern int mock_called_create_reply;
Test (new_request_replies, zero_reply)
{
    in_connector_t* in_conn = (in_connector_t*) malloc (sizeof(in_connector_t));
    mock_called_new_http_request = 0;
    mock_called_create_reply = 0;

    request_replies_t* rr = new_request_replies(in_conn, 0, NULL, FORWARD_MODE_SEQ);

    cr_assert_not_null(rr, "'new_request_replies' expected to return a non-null value");
    cr_assert (1 == mock_called_new_http_request, "Should have called 'new_http_request'. "
                                                  "Actual was:%d", mock_called_new_http_request);
    cr_assert (0 == mock_called_create_reply, "Should not have called ''create_replies."
                                                "Actual was:%d", mock_called_create_reply);
    cr_assert (0 == rr->out_connections, "Should have initialized 'out_connections' to '0'."
                                         "Instead value is:%d", rr->out_connections);
    cr_assert (FORWARD_MODE_SEQ == rr->forward_mode, "Should have initialized 'forward_mode' to 'FORWARD_MODE_SEQ'."
                                                     "Instead is:%d", rr->forward_mode);
    free (in_conn);
    free (rr->request);
    free (rr);
}

Test (new_request_replies, two_replies)
{
    in_connector_t* in_conn = (in_connector_t*) malloc (sizeof(in_connector_t));
    out_connector_t** out_conn = (out_connector_t**) malloc (sizeof(out_connector_t*) * 2);
    out_conn[0] = (out_connector_t*) malloc (sizeof(out_connector_t));
    out_conn[1] = (out_connector_t*) malloc (sizeof(out_connector_t));
    mock_called_new_http_request = 0;
    mock_called_create_reply = 0;

    request_replies_t* rr = new_request_replies(in_conn, 2, out_conn, FORWARD_MODE_SEQ);

    cr_assert (1 == mock_called_new_http_request, "Should have called 'new_http_request'. "
                                                  "Actual was:%d", mock_called_new_http_request);
    cr_assert (2 == mock_called_create_reply, "Should have called ''create_replies."
                                              "Actual was:%d", mock_called_create_reply);
    cr_assert (2 == rr->out_connections, "Should have initialized 'out_connections' to '0'."
                                         "Instead value is:%d", rr->out_connections);
    free (in_conn);
    free (rr->request);
    free (out_conn[0]);
    free (out_conn[1]);
    free (out_conn);
    free (rr);
}

extern int mock_called_release_request;
extern int mock_called_release_reply;
extern long mock_param1_release_request[];
Test (free_request_replies, zero_reply)
{
    in_connector_t* in_conn = (in_connector_t*) malloc (sizeof(in_connector_t));
    mock_called_release_request = 0;
    mock_called_release_reply = 0;
    request_replies_t* rr = new_request_replies(in_conn, 0, NULL, FORWARD_MODE_SEQ);

    rr = free_request_replies (rr);

    cr_assert_null (rr, "should be null");
    cr_assert (1 == mock_called_release_request, "Should have called 'new_http_request'. "
                                                  "Actual was:%d", mock_called_release_request);
    cr_assert (0 == mock_called_release_reply, "Should not have called ''create_replies."
                                              "Actual was:%d", mock_called_release_reply);
    cr_assert ((long)rr->request == mock_param1_release_request[0], "Should have passed the request in parameter");
}

Test (free_request_replies, two_replies) {
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    out_connector_t **out_conn = (out_connector_t **) malloc(sizeof(out_connector_t *) * 2);
    out_conn[0] = (out_connector_t *) malloc(sizeof(out_connector_t));
    out_conn[1] = (out_connector_t *) malloc(sizeof(out_connector_t));
    mock_called_release_request = 0;
    mock_called_release_reply = 0;
    request_replies_t *rr = new_request_replies(in_conn, 2, out_conn, FORWARD_MODE_SEQ);

    rr = free_request_replies (rr);

    cr_assert_null (rr, "should be null");
    cr_assert (1 == mock_called_release_request, "Should have called 'new_http_request'. "
                                                  "Actual was:%d", mock_called_release_request);
    cr_assert (2 == mock_called_release_reply, "Should have called ''create_replies."
                                              "Actual was:%d", mock_called_release_reply);
}

extern void join_client_threads (request_replies_t* request);
extern int mock_called_pthread_join;
Test (join_client_threads, zero_reply)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 0, NULL, FORWARD_MODE_SEQ);
    mock_called_pthread_join = 0;

    join_client_threads (rr);

    cr_assert (0 == mock_called_pthread_join, "Should not have been called 'pthread_join_wraaper'."
                                              "Actual was:%d", mock_called_pthread_join);
    free_request_replies (rr);
}

Test (join_client_threads, two_replies)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    out_connector_t **out_conn = (out_connector_t **) malloc(sizeof(out_connector_t *) * 2);
    out_conn[0] = (out_connector_t *) malloc(sizeof(out_connector_t));
    out_conn[1] = (out_connector_t *) malloc(sizeof(out_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 2, out_conn, FORWARD_MODE_SEQ);
    mock_called_pthread_join = 0;

    join_client_threads (rr);

    cr_assert (2 == mock_called_pthread_join, "Should not have been called 'pthread_join_wraaper'."
                                              "Actual was:%d", mock_called_pthread_join);
    free_request_replies (rr);
}

extern int mock_called_pthread_create;
Test (wait_4_all_client_send_receive_to_complete, call)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 0, NULL, FORWARD_MODE_SEQ);
    mock_called_pthread_create = 0;

    wait_4_all_client_send_receive_to_complete (rr);

    cr_assert (1 == mock_called_pthread_create, "Should have called 'pthread_create'."
                                                "Actual was:%d", mock_called_pthread_create);
    free_request_replies (rr);
}

extern void synchronize_all_senders (request_replies_t* rr);
Test (synchronize_all_senders, FORWARD_MODE_SYNC_zero_server_connection)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 0, NULL, FORWARD_MODE_SYNC);
    mock_called_pthread_join = 0;
    mock_called_pthread_create = 0;

    synchronize_all_senders (rr);

    cr_assert (0 == mock_called_pthread_join, "Should not have called 'pthread_join'."
                                              "Actual was:%d", mock_called_pthread_join);
    cr_assert (0 == mock_called_pthread_create, "Should not have called 'pthread_create'."
                                              "Actual was:%d", mock_called_pthread_create);
    free_request_replies (rr);
}

Test (synchronize_all_senders, FORWARD_MODE_SYNC_two_server_connections)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    out_connector_t **out_conn = (out_connector_t **) malloc(sizeof(out_connector_t *) * 2);
    out_conn[0] = (out_connector_t *) malloc(sizeof(out_connector_t));
    out_conn[1] = (out_connector_t *) malloc(sizeof(out_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 2, out_conn, FORWARD_MODE_SYNC);
    mock_called_pthread_join = 0;
    mock_called_pthread_create = 0;

    synchronize_all_senders (rr);

    cr_assert (2 == mock_called_pthread_join, "Should not have called 'pthread_join'."
                                              "Actual was:%d", mock_called_pthread_join);
    cr_assert (0 == mock_called_pthread_create, "Should not have called 'pthread_create'."
                                                "Actual was:%d", mock_called_pthread_create);
    free_request_replies (rr);
}

Test (synchronize_all_senders, FORWARD_MODE_SEQ)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 0, NULL, FORWARD_MODE_SEQ);
    mock_called_pthread_join = 0;
    mock_called_pthread_create = 0;

    synchronize_all_senders (rr);

    cr_assert (0 == mock_called_pthread_join, "Should not have called 'pthread_join'."
                                              "Actual was:%d", mock_called_pthread_join);
    cr_assert (0 == mock_called_pthread_create, "Should not have called 'pthread_create'."
                                                "Actual was:%d", mock_called_pthread_create);
    free_request_replies (rr);
}

Test (synchronize_all_senders, FORWARD_MODE_ASYNC_zero_server_connection)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 0, NULL, FORWARD_MODE_ASYNC);
    mock_called_pthread_join = 0;
    mock_called_pthread_create = 0;

    synchronize_all_senders (rr);

    cr_assert (0 == mock_called_pthread_join, "Should not have called 'pthread_join'."
                                              "Actual was:%d", mock_called_pthread_join);
    cr_assert (1 == mock_called_pthread_create, "Should have called 'pthread_create'."
                                                "Actual was:%d", mock_called_pthread_create);
    free_request_replies (rr);
}

Test (synchronize_all_senders, FORWARD_MODE_ASYNC_rwo_server_connections)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    out_connector_t **out_conn = (out_connector_t **) malloc(sizeof(out_connector_t *) * 2);
    out_conn[0] = (out_connector_t *) malloc(sizeof(out_connector_t));
    out_conn[1] = (out_connector_t *) malloc(sizeof(out_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 2, out_conn, FORWARD_MODE_ASYNC);
    mock_called_pthread_join = 0;
    mock_called_pthread_create = 0;

    synchronize_all_senders (rr);

    cr_assert (0 == mock_called_pthread_join, "Should not have called 'pthread_join'."
                                              "Actual was:%d", mock_called_pthread_join);
    cr_assert (1 == mock_called_pthread_create, "Should have called 'pthread_create'."
                                                "Actual was:%d", mock_called_pthread_create);
    free_request_replies (rr);
}

extern void release_buffer_after_processing (request_replies_t* rr);
Test (release_buffer_after_processing, zero_server_connection)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 0, NULL, FORWARD_MODE_ASYNC);
    mock_called_release_request = 0;
    mock_called_release_reply = 0;

    release_buffer_after_processing (rr);

    cr_assert (1 == mock_called_release_request, "Should have called 'release_request'."
                                                 "Actual was:%d", mock_called_release_request);
    cr_assert (0 == mock_called_release_reply, "Should not have called 'release_reply'."
                                               "Actual was:%d", mock_called_release_reply);
    free_request_replies (rr);
}

Test (release_buffer_after_processing, two_server_connections)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    out_connector_t **out_conn = (out_connector_t **) malloc(sizeof(out_connector_t *) * 2);
    out_conn[0] = (out_connector_t *) malloc(sizeof(out_connector_t));
    out_conn[1] = (out_connector_t *) malloc(sizeof(out_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 2, out_conn, FORWARD_MODE_ASYNC);
    mock_called_release_request = 0;
    mock_called_release_reply = 0;

    release_buffer_after_processing (rr);

    cr_assert (1 == mock_called_release_request, "Should have called 'release_request'."
                                                 "Actual was:%d", mock_called_release_request);
    cr_assert (2 == mock_called_release_reply, "Should have called 'release_reply'."
                                               "Actual was:%d", mock_called_release_reply);
    free_request_replies (rr);
}

extern int mock_called_receive_new_request_from_client;
Test (accept_opening_request_from_client, call)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 0, NULL, FORWARD_MODE_ASYNC);
    mock_called_receive_new_request_from_client = 0;

    accept_opening_request_from_client (rr);

    cr_assert (1 == mock_called_receive_new_request_from_client, "Should have called 'receive_new_request_from_client',"
                                                                 "Actual was:%d", mock_called_receive_new_request_from_client);
    free_request_replies (rr);
}

extern void forward_request_to_all_servers (request_replies_t *rr);
extern int mock_called_send_request_to_server_and_wait_reply;
Test (forward_request_to_all_servers, zero_server_connection)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 0, NULL, FORWARD_MODE_ASYNC);
    mock_called_send_request_to_server_and_wait_reply = 0;

    forward_request_to_all_servers (rr);

    cr_assert (0 == mock_called_send_request_to_server_and_wait_reply, "Should not have called 'send_request_to_server_and_wait_reply'."
                                                                      "Actual was:%d", mock_called_send_request_to_server_and_wait_reply);
    free_request_replies (rr);
}

Test (forward_request_to_all_servers, two_server_connections)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    out_connector_t **out_conn = (out_connector_t **) malloc(sizeof(out_connector_t *) * 2);
    out_conn[0] = (out_connector_t *) malloc(sizeof(out_connector_t));
    out_conn[1] = (out_connector_t *) malloc(sizeof(out_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 2, out_conn, FORWARD_MODE_ASYNC);
    mock_called_send_request_to_server_and_wait_reply = 0;

    forward_request_to_all_servers (rr);

    cr_assert (2 == mock_called_send_request_to_server_and_wait_reply, "Should have called 'send_request_to_server_and_wait_reply'."
                                                                       "Actual was:%d", mock_called_send_request_to_server_and_wait_reply);
    free_request_replies (rr);
}

extern int mock_called_reply_to_client;
extern void forward_replies (request_replies_t* rr);
Test (forward_replies, zero_server_defined)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 0, NULL, FORWARD_MODE_ASYNC);
    mock_called_reply_to_client = 0;

    forward_replies (rr);

    cr_assert (0 == mock_called_reply_to_client, "Should not have called 'reply_to_client'."
                                          "Actual was:%d", mock_called_reply_to_client);
}

extern long mock_param_reply_to_client[];
Test (forward_replies, two_servers_defined)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    out_connector_t **out_conn = (out_connector_t **) malloc(sizeof(out_connector_t *) * 2);
    out_conn[0] = (out_connector_t *) malloc(sizeof(out_connector_t));
    out_conn[1] = (out_connector_t *) malloc(sizeof(out_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 2, out_conn, FORWARD_MODE_ASYNC);

    forward_replies (rr);

    cr_assert (2 == mock_called_reply_to_client, "Should not have called 'reply_to_client'."
                                                 "Actual was:%d", mock_called_reply_to_client);
    for (int i=0; i < 2; i++) {
        cr_assert ((long) rr->replies[i] == mock_param_reply_to_client[i], "Should have passed the address to 'reply' at"
                                                                        "index:%d", i);
    }
}

extern int mock_called_decode_request_message_header;
extern int mock_called_process_request_message_body;
Test (strategy_sequential_request_replies, call)
{
    in_connector_t *in_conn = (in_connector_t *) malloc(sizeof(in_connector_t));
    out_connector_t **out_conn = (out_connector_t **) malloc(sizeof(out_connector_t *) * 1);
    out_conn[0] = (out_connector_t *) malloc(sizeof(out_connector_t));
    request_replies_t* rr = new_request_replies(in_conn, 1, out_conn, FORWARD_MODE_SYNC);
    mock_called_decode_request_message_header = 0;
    mock_called_process_request_message_body = 0;
    mock_called_send_request_to_server_and_wait_reply = 0;
    mock_called_pthread_join = 0;
    mock_called_reply_to_client = 0;
    mock_called_release_reply = 0;

    strategy_sequential_request_replies (rr);

    cr_assert (1 == mock_called_decode_request_message_header, "Should have called 'decode_request_message_header'."
                                                               "Actual was:%d", mock_called_decode_request_message_header);

    cr_assert (1 == mock_called_process_request_message_body, "Should have called 'process_request_message_body'."
                                                               "Actual was:%d", mock_called_process_request_message_body);
    cr_assert (1 == mock_called_send_request_to_server_and_wait_reply, "Should have called 'send_request_to_server_and_wait_reply'."
                                                              "Actual was:%d", mock_called_send_request_to_server_and_wait_reply);
    cr_assert (1 == mock_called_pthread_join, "Should have called 'pthread_joi'."
                                                                       "Actual was:%d", mock_called_pthread_join);
    cr_assert (1 == mock_called_reply_to_client, "Should have called 'reply_to_client'."
                                              "Actual was:%d", mock_called_reply_to_client);
    cr_assert (1 == mock_called_release_reply, "Should have called 'release_reply'."
                                                 "Actual was:%d", mock_called_release_reply);
}