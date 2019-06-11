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
    rr = free_request_replies (rr);
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
    rr = free_request_replies (rr);
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
}