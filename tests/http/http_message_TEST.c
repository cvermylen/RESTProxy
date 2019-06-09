#include "../../src/http/http_message.h"
#include "../../src/http/http_first_line.h"
#include "../../src/buffers/shared_buffers.h"
#include "../mocks/mock_socket_connector.h"
#include <str_stack.h>
#include <criterion/criterion.h>

extern int http_message_buffer_size;

Test(http_message, init)
{
	char* buff = (char*)malloc(sizeof(char));

    http_message_t* msg = new_http_message(0, NULL, 0);

    cr_assert(msg != NULL, "init expected to return a non-NULL value");

	free(buff);
	http_message_free(msg);
}

Test(http_message, init2)
{
    http_message_buffer_size = 0;  //Expect to have only one buffer
    http_message_t* msg = new_http_message(0, NULL, 0);

    cr_assert(msg != NULL, "init expected to return a non-NULL value");
    cr_assert(msg->buffers != NULL, "Need a circular buffer");
    cr_assert(msg->status == HTTP_MSG_STATUS_HEADER, "Status should be initialized");
    http_message_free(msg);
}

extern int mock_called_set_data_size_for_last_received_buffer;
extern int mock_called_alloc_entry_in_circular_buffer;
extern int mock_result_read_into_next_buffer;
/*!
 * Simulate 1 received buffer
 */
Test(read_from_source, read_in_one_buffer)
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
	http_message_free(msg);
}

Test(http_message, 2_read_should_block)
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
    http_message_receive_body(msg);
}

/*Test(http_message, update_status_1)
{
	http_message_t* requestMessage = (http_message_t*)malloc(sizeof(http_message_t));
	requestMessage->status = HTTP_MSG_STATUS_INIT;
	requestMessage->buffer = "GET / HTTP1/1";
	http_message_update_status(requestMessage, 0, strlen(requestMessage->buffer));
	cr_assert(HTTP_MSG_STATUS_INIT == requestMessage->status, "expected status HTTP_MSG_STATUS_INIT, not:%d", requestMessage->status);
}*/

/*Test(http_message, update_status_2)
{
	http_message_t* requestMessage = (http_message_t*)malloc(sizeof(http_message_t));
	requestMessage->status = HTTP_MSG_STATUS_INIT;
	requestMessage->buffer = "XX / HTTP1/1";
	http_message_update_status(requestMessage, 0, strlen(requestMessage->buffer));
	cr_assert(HTTP_MSG_STATUS_INIT == requestMessage->status, "expected status HTTP_MSG_STATUS_INIT, not:%d", requestMessage->status);
}*/

/*Test(http_message, update_status_3)
{
	http_message_t* requestMessage = (http_message_t*)malloc(sizeof(http_message_t));
	requestMessage->status = HTTP_MSG_STATUS_INIT;
	requestMessage->buffer = "POST / HTTP1/1";
	http_message_update_status(requestMessage, 0, strlen(requestMessage->buffer));
	cr_assert(HTTP_MSG_STATUS_INIT == requestMessage->status, "expected status HTTP_MSG_STATUS_INIT, not:%d", requestMessage->status);
}*/


/*Test(http_message, update_status_4)
{
	http_message_t* requestMessage = (http_message_t*)malloc(sizeof(http_message_t));
	requestMessage->status = HTTP_MSG_STATUS_HEADER;
	
	char buffer[] = "Host: Mac";
	requestMessage->buffer = (char*)malloc(sizeof(char) * strlen(buffer));
	strcpy(requestMessage->buffer, buffer);
	http_message_update_status(requestMessage, 0, strlen(requestMessage->buffer));
	cr_assert(HTTP_MSG_STATUS_HEADER == requestMessage->status, "expected status HTTP_MSG_STATUS_HEADER, not:%d", requestMessage->status);
}*/


/*Test(http_message, update_status_5)
{
	http_message_t* requestMessage = (http_message_t*)malloc(sizeof(http_message_t));
	requestMessage->status = HTTP_MSG_STATUS_HEADER;
	
	char buffer[] = "Content-Length: 987";
	requestMessage->buffer = (char*)malloc(sizeof(char) * strlen(buffer));
	strcpy(requestMessage->buffer, buffer);
	http_message_update_status(requestMessage, 0, strlen(requestMessage->buffer));
	cr_assert(HTTP_MSG_STATUS_HEADER == requestMessage->status, "expected status HTTP_MSG_STATUS_LENGTH, not:%d", requestMessage->status);
	cr_assert(987 == requestMessage->body_length, "expected body length of 987, not:%d", requestMessage->body_length);
}*/

