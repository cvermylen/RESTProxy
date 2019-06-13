#include "../../src/http/request_replies.h"
#include "mock_defs.h"

int mock_called_new_request_replies;
long mock_param_1_new_request_replies[MAX_CALLS_SAME_FUNCTION];
int mock_param_2_new_request_replies[MAX_CALLS_SAME_FUNCTION];
long mock_param_3_new_request_replies[MAX_CALLS_SAME_FUNCTION];
int mock_param_4_new_request_replies[MAX_CALLS_SAME_FUNCTION];
request_replies_t* mock_result_new_request_replies;
request_replies_t* new_request_replies (in_connector_t* connector_def, int number_of_servers, out_connector_t** server_conns, int forward_mode)
{
    mock_param_1_new_request_replies[mock_called_new_request_replies] = (long) connector_def;
    mock_param_2_new_request_replies[mock_called_new_request_replies] = number_of_servers;
    mock_param_3_new_request_replies[mock_called_new_request_replies] = (long)server_conns;
    mock_param_4_new_request_replies[mock_called_new_request_replies] = forward_mode;
    mock_called_new_request_replies += 1;

    return mock_result_new_request_replies;
}

int mock_called_accept_opening_request_from_client;
long mock_param_1_accept_opening_request_from_client[MAX_CALLS_SAME_FUNCTION];
int mock_result_accept_opening_request_from_client;
int accept_opening_request_from_client (request_replies_t* rr)
{
    mock_param_1_accept_opening_request_from_client[mock_called_accept_opening_request_from_client] = (long)rr;
    mock_called_accept_opening_request_from_client += 1;
    return mock_result_accept_opening_request_from_client;
}

int mock_called_strategy_sequential_request_replies;
long mock_param_1_strategy_sequential_request_replies[MAX_CALLS_SAME_FUNCTION];
void strategy_sequential_request_replies (request_replies_t* rr)
{
    mock_param_1_strategy_sequential_request_replies[mock_called_strategy_sequential_request_replies] = (long)rr;
    mock_called_strategy_sequential_request_replies += 1;
}

int mock_called_release_buffer_after_processing;
long mock_param_1_release_buffer_after_processing[MAX_CALLS_SAME_FUNCTION];
void release_buffer_after_processing(request_replies_t* rr)
{
    mock_param_1_release_buffer_after_processing[mock_called_release_buffer_after_processing] = (long)rr;
    mock_called_release_buffer_after_processing += 1;
}

int mock_called_forward_request_to_all_servers;
long mock_param_1_forward_request_to_all_servers[MAX_CALLS_SAME_FUNCTION];
void forward_request_to_all_servers(request_replies_t *rr)
{
    mock_param_1_forward_request_to_all_servers[mock_called_forward_request_to_all_servers] = (long)rr;
}

int mock_called_wait_4_all_client_send_receive_to_complete;
int mock_result_wait_4_all_client_send_receive_to_complete;
long mock_param_1_wait_4_all_client_send_receive_to_complete[MAX_CALLS_SAME_FUNCTION];
int wait_4_all_client_send_receive_to_complete(request_replies_t *rr)
{
    mock_param_1_wait_4_all_client_send_receive_to_complete[mock_called_wait_4_all_client_send_receive_to_complete] = (long)rr;
    mock_called_wait_4_all_client_send_receive_to_complete += 1;
    return mock_result_wait_4_all_client_send_receive_to_complete;
}

int mock_called_free_request_replies;
long mock_param_1_free_request_replies[MAX_CALLS_SAME_FUNCTION];
request_replies_t* mock_result_free_request_replies;
request_replies_t* free_request_replies (request_replies_t* rr)
{
    mock_param_1_free_request_replies[mock_called_free_request_replies] = (long)rr;
    mock_called_free_request_replies += 1;
    return mock_result_free_request_replies;
}
