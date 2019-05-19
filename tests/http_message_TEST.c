#include "../src/http/http_message.h"
#include "../src/http/http_first_line.h"
#include "../src/frame_buffers/shared_buffers.h"
#include "mocks/mock_socket_connector.h"
#include <str_stack.h>
#include <criterion/criterion.h>

extern int http_message_buffer_size;

Test(http_message, init)
{
	char* buff = (char*)malloc(sizeof(char));

    http_message_t* msg = http_message_init(0);

    cr_assert(msg != NULL, "init expected to return a non-NULL value");

	free(buff);
	http_message_free(msg);
}

Test(http_message, init2)
{
    http_message_buffer_size = 0;  //Expect to have only one buffer
    http_message_t* msg = http_message_init(0);

    cr_assert(msg != NULL, "init expected to return a non-NULL value");
    cr_assert(msg->last_received == msg->last_sent, "With not buffer, both should be equal");
    cr_assert(msg->status == HTTP_MSG_STATUS_HEADER, "Status should be initialized");
    http_message_free(msg);
}

/*!
 * Define an empty list of buffers (override BUFFER_SIZE to 0)
 */
Test(read_from_source, read_in_one_buffer)
{
    http_message_buffer_size = 1;  // 2 ^ 1 = 2 buffers
    http_message_t* msg = http_message_init(0);
    mock_socket_connect_stack_to_fd(0);
    char buff[] = "dummy";
    mock_socket_push_buffer(0, buff);

    int n = read_next_buffer_from_source(msg);
	cr_assert(5 == n, "expected length 5, not: %d", n);
	cr_assert(msg->last_received == 1, "Should have been updated to 1, currently is: %ud", msg->last_received);
	cr_assert(msg->last_sent == 0, "Last_sent expected to be 0, not:%d", msg->last_sent);
	cr_assert(msg->data_sizes[1] == n, "Data size for buffer[1] should be 5, not: %d", msg->data_sizes[1]);
	http_message_free(msg);
}

Test(http_message, 2_read_should_block)
{
    http_message_buffer_size = 1;  // 2 ^ 1 = 2 buffers
    http_message_t* msg = http_message_init(0);
    mock_socket_connect_stack_to_fd(0);
    char buff2[] = "Second Line";
    mock_socket_push_buffer(0, buff2);
    char buff1[] = "First Line";
    mock_socket_push_buffer(0, buff1);

	read_next_buffer_from_source(msg);
	int m = read_next_buffer_from_source(msg);

	cr_assert(0 == m, "Not buffer available for the second read, expected to return '0', not:%d", m);
	http_message_free(msg);
}

Test(free_buffer, free_1)
{
    http_message_buffer_size = 1;  // 2 ^ 1 = 2 buffers
    http_message_t* msg = http_message_init(0);
    msg->buffers[0] = alloc_buffer();
    http_message_free_buffer(msg);

    cr_assert(1 == msg->last_sent, "Expected last message to be 1, not:%d", msg->last_sent);
    cr_assert(1 == msg->last_received, "Last received should not have moved: %d", msg->last_received);
}

Test(receive_body, has_already_received_body_content)
{
    http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
    msg->raw_message_length = 10;
    msg->body_length = 5;
    receive_body(0, msg, 5);
}

/*Test(http_message, update_status_1)
{
	http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
	msg->status = HTTP_MSG_STATUS_INIT;
	msg->buffer = "GET / HTTP1/1";
	http_message_update_status(msg, 0, strlen(msg->buffer));
	cr_assert(HTTP_MSG_STATUS_INIT == msg->status, "expected status HTTP_MSG_STATUS_INIT, not:%d", msg->status);
}*/

/*Test(http_message, update_status_2)
{
	http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
	msg->status = HTTP_MSG_STATUS_INIT;
	msg->buffer = "XX / HTTP1/1";
	http_message_update_status(msg, 0, strlen(msg->buffer));
	cr_assert(HTTP_MSG_STATUS_INIT == msg->status, "expected status HTTP_MSG_STATUS_INIT, not:%d", msg->status);
}*/

/*Test(http_message, update_status_3)
{
	http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
	msg->status = HTTP_MSG_STATUS_INIT;
	msg->buffer = "POST / HTTP1/1";
	http_message_update_status(msg, 0, strlen(msg->buffer));
	cr_assert(HTTP_MSG_STATUS_INIT == msg->status, "expected status HTTP_MSG_STATUS_INIT, not:%d", msg->status);
}*/


/*Test(http_message, update_status_4)
{
	http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
	msg->status = HTTP_MSG_STATUS_HEADER;
	
	char buffer[] = "Host: Mac";
	msg->buffer = (char*)malloc(sizeof(char) * strlen(buffer));
	strcpy(msg->buffer, buffer);
	http_message_update_status(msg, 0, strlen(msg->buffer));
	cr_assert(HTTP_MSG_STATUS_HEADER == msg->status, "expected status HTTP_MSG_STATUS_HEADER, not:%d", msg->status);
}*/


/*Test(http_message, update_status_5)
{
	http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
	msg->status = HTTP_MSG_STATUS_HEADER;
	
	char buffer[] = "Content-Length: 987";
	msg->buffer = (char*)malloc(sizeof(char) * strlen(buffer));
	strcpy(msg->buffer, buffer);
	http_message_update_status(msg, 0, strlen(msg->buffer));
	cr_assert(HTTP_MSG_STATUS_HEADER == msg->status, "expected status HTTP_MSG_STATUS_LENGTH, not:%d", msg->status);
	cr_assert(987 == msg->body_length, "expected body length of 987, not:%d", msg->body_length);
}*/

