#include "../../src/http/request_replies.h"
#include "../../src/route.h"
#include <criterion/criterion.h>

extern int mock_called_new_http_request;
extern int mock_called_create_reply;
Test (new_request_replies, zero_reply)
{
    in_connector_t* in_conn = (in_connector_t*) malloc (sizeof(in_connector_t));
    mock_called_new_http_request = 0;
    mock_called_create_reply = 0;

    request_replies_t* rr = new_request_replies(in_conn, 0, NULL);

    cr_assert_not_null(rr, "'new_request_replies' expected to return a non-null value");
    cr_assert (1 == mock_called_new_http_request, "Should have called 'new_http_request'. "
                                                  "Actual was:%d", mock_called_new_http_request);
    cr_assert (0 == mock_called_create_reply, "Should not have called ''create_replies."
                                                "Actual was:%d", mock_called_create_reply);
    cr_assert (0 == rr->out_connections, "Should have initialized 'out_connections' to '0'."
                                         "Instead value is:%d", rr->out_connections);
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

    request_replies_t* rr = new_request_replies(in_conn, 2, out_conn);

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
    request_replies_t* rr = new_request_replies(in_conn, 0, NULL);

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
    request_replies_t *rr = new_request_replies(in_conn, 2, out_conn);

    rr = free_request_replies (rr);

    cr_assert_null (rr, "should be null");
    cr_assert (1 == mock_called_release_request, "Should have called 'new_http_request'. "
                                                  "Actual was:%d", mock_called_release_request);
    cr_assert (2 == mock_called_release_reply, "Should have called ''create_replies."
                                              "Actual was:%d", mock_called_release_reply);
}