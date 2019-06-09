#include "../../src/http/http_message.h"
#include "../../src/http/http_first_line.h"
#include "../../src/buffers/shared_buffers.h"
#include "../mocks/mock_socket_connector.h"
#include <str_stack.h>
#include <criterion/criterion.h>

extern int http_message_buffer_size;

Test(new_http_message, init)
{
	char* buff = (char*)malloc(sizeof(char));

    http_message_t* msg = new_http_message(0, NULL, 0);

    cr_assert(msg != NULL, "init expected to return a non-NULL value");

	free(buff);
	http_message_free(msg);
}

Test(new_http_message, init2)
{
    http_message_t* msg = new_http_message(0, NULL, 0);

    cr_assert(msg != NULL, "init expected to return a non-NULL value");
    cr_assert(msg->buffers != NULL, "Need a circular buffer");
    cr_assert(msg->status == HTTP_MSG_STATUS_INIT, "Status should be initialized");
    http_message_free(msg);
}

Test (new_http_message, bad_buffer_size)
{
    http_message_buffer_size = 0;
    http_message_t* msg = new_http_message (0, NULL, 0);
    http_message_buffer_size = BUFFER_SIZE;

    cr_assert_null(msg, "Won't create a message if size of buffer is 0");
}

extern int mock_called_set_data_size_for_last_received_buffer;
extern int mock_called_alloc_entry_in_circular_buffer;
extern int mock_result_read_into_next_buffer;
extern int mock_called_read_into_next_buffer;
/*!
 * Simulate 1 received buffer
 */
Test(read_next_buffer_from_source, read_in_one_buffer)
{
    http_message_buffer_size = 1;  // 2 ^ 1 = 2 buffers
    http_message_t* msg = new_http_message(0, NULL, 0);
    mock_socket_connect_stack_to_fd(0);
    char buff[] = "dummy";
    mock_socket_push_buffer(0, buff);
    mock_called_set_data_size_for_last_received_buffer = 0;
    mock_called_alloc_entry_in_circular_buffer = 0;
    mock_result_read_into_next_buffer = 5;

    int n = read_next_buffer_from_source(msg);
	cr_assert(5 == n, "expected length 5, not: %d", n);
	cr_assert(mock_called_set_data_size_for_last_received_buffer == 1, "Function should have been called");
	cr_assert(mock_called_alloc_entry_in_circular_buffer == 1, "Should have been called: 'alloc_entry_in_circular_buffer'");
	cr_assert (mock_called_read_into_next_buffer == 1, "Should have called 'read_into_next_buffer'");
	http_message_free(msg);
}

Test(read_next_buffer_from_source, 2_read_should_block)
{
    http_message_buffer_size = 1;  // 2 ^ 1 = 2 buffers
    http_message_t* msg = new_http_message(0, NULL, 0);
    mock_socket_connect_stack_to_fd(0);
    char buff2[] = "Second Line";
    mock_socket_push_buffer(0, buff2);
    char buff1[] = "First Line";
    mock_socket_push_buffer(0, buff1);
    mock_called_alloc_entry_in_circular_buffer = 0;

	read_next_buffer_from_source(msg);
	int m = read_next_buffer_from_source(msg);

	cr_assert(0 == m, "Not buffer available for the second read, expected to return '0', not:%d", m);
    cr_assert(mock_called_alloc_entry_in_circular_buffer == 2, "Should have been called: 'alloc_entry_in_circular_buffer'");
	http_message_free(msg);
}

extern int mock_result_alloc_entry_in_circular_buffer;
Test (read_next_buffer_from_source, no_buffer_available)
{
    http_message_t* msg = new_http_message(0, NULL, 0);
    mock_result_alloc_entry_in_circular_buffer = -1;

    int res = read_next_buffer_from_source (msg);

    cr_assert (0 == res, "Should not actually execute any read if no buffer is available. Actual result was:%d", res);
    http_message_free(msg);
}

extern int mock_called_http_decode_request_type;
Test (http_message_decode_request_type, call)
{
    http_message_t* msg = new_http_message(0, NULL, 0);

    int res = http_message_decode_request_type (msg);

    cr_assert (1 == mock_called_http_decode_request_type, "Should have called 'http_decode_request_type'. Actual was:%d", mock_called_http_decode_request_type);
    http_message_free(msg);
}

extern int mock_called_http_decode_response_type;
Test (http_message_decode_response_type, call)
{
    http_message_t* msg = new_http_message(0, NULL, 0);

    int res = http_message_decode_response_type (msg);

    cr_assert (1 == mock_called_http_decode_response_type, "Should have called 'http_decode_response_type'. Actual was:%d", mock_called_http_decode_response_type);
    http_message_free(msg);
}

extern int mock_called_decode_http_headers;
extern int mock_called_decode_body_length;
Test (decode_http_message_header, call)
{
    http_message_t* msg = new_http_message(0, NULL, 0);

    decode_http_message_header (msg);

    cr_assert (1 == mock_called_decode_http_headers, "Should have called 'decode_http_headers'. Actual was:%d", mock_called_decode_http_headers);
    cr_assert (1 == mock_called_decode_body_length, "Should have called 'decode_body_length'. Actual was:%d", mock_called_decode_body_length);
    cr_assert (HTTP_MSG_STATUS_HEADER_COMPLETE == msg->status, "Status expected to be 'HTTP_MSG_STATUS_HEADER_COMPLETE', not:%d", msg->status);
    http_message_free(msg);
}

extern int mock_called_free_circular_buffer;
extern int mock_called_http_headers_free;

Test(free_buffer, free_1)
{
    http_message_buffer_size = 1;  // 2 ^ 1 = 2 buffers
    http_message_t* msg = new_http_message(0, NULL, 0);
    mock_called_free_circular_buffer = 0;
    mock_called_http_headers_free = 0;

    http_message_free(msg);

    cr_assert(mock_called_free_circular_buffer == 1, "Should have been called: 'free_circular_buffer'");
    cr_assert(mock_called_http_headers_free == 1, "Should have been called: 'http_headers_free'");
}

Test(http_message_receive_body, has_already_received_body_content)
{
    http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
    msg->raw_message_length = 10;
    msg->body_length = 5;
    mock_result_alloc_entry_in_circular_buffer = -1;

    http_message_receive_body(msg);

    cr_assert (1 == mock_called_alloc_entry_in_circular_buffer, "Should have called 'alloc_entry_in_circular_buffer', Actual was:%d", mock_called_alloc_entry_in_circular_buffer);
    cr_assert (HTTP_MSG_STATUS_BODY_COMPLETE == msg->status, "Expected status 'HTTP_MSG_STATUS_BODY_COMPLETE', not:%d", msg->status);
    http_message_free(msg);
}

int mock_called_send_data = 0;
int mock_result_send_data;
int mock_send_data ()
{
    mock_called_send_data += 1;
    return mock_result_send_data;
}
extern int mock_called_is_empty_circular_buffer;
extern int mock_result_is_empty_circular_buffer;
extern int mock_called_buffer_has_been_sent;
Test (send_next_buffer_to_destination, call)
{
    http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
    mock_result_is_empty_circular_buffer = 0;
    mock_called_is_empty_circular_buffer = 0;
    mock_called_send_data = 0;
    mock_called_buffer_has_been_sent = 0;

    send_next_buffer_to_destination (mock_send_data, NULL, msg, 0);

    cr_assert (1 == mock_called_is_empty_circular_buffer, "Should have called 'is_empty_circular_buffer'. Actual was:%d", mock_called_is_empty_circular_buffer);
    cr_assert (1 == mock_called_send_data, "Should have called 'send_data'. Actual was:%d", mock_called_send_data);
    cr_assert (0 == mock_called_buffer_has_been_sent, "Should NOT have been called 'buffer_has_been_sent'. Actual was:%d", mock_called_buffer_has_been_sent);
    http_message_free(msg);
}

Test (send_next_buffer_to_destination, no_call)
{
    http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
    mock_result_is_empty_circular_buffer = 1;
    mock_called_is_empty_circular_buffer = 0;
    mock_called_send_data = 0;
    mock_called_buffer_has_been_sent = 0;

    send_next_buffer_to_destination (mock_send_data, NULL, msg, 0);

    cr_assert (1 == mock_called_is_empty_circular_buffer, "Should have called 'is_empty_circular_buffer'. Actual was:%d", mock_called_is_empty_circular_buffer);
    cr_assert (0 == mock_called_send_data, "Should NOT have been called 'send_data'. Actual was:%d", mock_called_send_data);
    cr_assert (0 == mock_called_buffer_has_been_sent, "Should NOT have been called 'buffer_has_been_sent'. Actual was:%d", mock_called_buffer_has_been_sent);
    http_message_free(msg);
}

Test (send_next_buffer_to_destination, all_sent)
{
    http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
    mock_result_is_empty_circular_buffer = 0;
    mock_called_is_empty_circular_buffer = 0;
    mock_called_send_data = 0;
    mock_called_buffer_has_been_sent = 0;

    send_next_buffer_to_destination (mock_send_data, NULL, msg, 1);

    cr_assert (1 == mock_called_is_empty_circular_buffer, "Should have called 'is_empty_circular_buffer'. Actual was:%d", mock_called_is_empty_circular_buffer);
    cr_assert (1 == mock_called_send_data, "Should have called 'send_data'. Actual was:%d", mock_called_send_data);
    cr_assert (1 == mock_called_buffer_has_been_sent, "Should have called 'buffer_has_been_sent'. Actual was:%d", mock_called_buffer_has_been_sent);
    http_message_free(msg);
}
