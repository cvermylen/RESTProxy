#include "../src/http_message.h"
#include "../src/http_first_line.h"
#include <str_stack.h>
#include <criterion/criterion.h>

extern char* mock_socket_buffer;

Test(http_message, init)
{
	http_message_t* msg = NULL;
	char* buff = (char*)malloc(sizeof(char));
	msg = http_message_init(0, buff, 0, 0);
	cr_assert(msg != NULL, "init expected to return a non-NULL value");
	free(buff);
	http_message_free(msg);
}

/*Test(http_message, line_length1)
{
	char text[] = "a short line\nfollowed by another";
	int first = get_line_length(text);
	cr_assert(12 == first, "size of 'a short line' expected to be 12, not:%d", first);
	int second = get_line_length(text+(sizeof(char) *13));
	cr_assert(19 == second, "size of 'followed by another' expected to be 19, not:%d", second);
}*/

/*Test(http_message, read_from_buffer_0)
{
	http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
	msg->buffer = (char*)malloc(sizeof(char) * 1024);
	mock_socket_buffer = "dummy";
	int n = read_from_buffer(0, msg, 0);
	cr_assert(5 == n, "expected length 5, not: %d", n);
}*/

/*Test(http_message, read_from_buffer_1)
{
	http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
	msg->buffer = (char*)malloc(sizeof(char) * 1024);
	strcpy(msg->buffer, "two words\n");
	msg->raw_message_length = strlen(msg->buffer);
	mock_socket_buffer = NULL;
	int n = read_from_buffer(0, msg, 3);
	cr_assert(6 == n, "expected length 6, not: %d", n);
}*/

/*Test(http_message, read_from_buffer_2)
{
	http_message_t* msg = (http_message_t*)malloc(sizeof(http_message_t));
	msg->buffer = (char*)malloc(sizeof(char) * 1024);
	strcpy(msg->buffer, "first line\nthen a second line");
	msg->raw_message_length = 29;
	mock_socket_buffer = NULL;
	int n = read_from_buffer(0, msg, 0);
	int m = read_from_buffer(0, msg, n+1);
	cr_assert(18 == m, "expected length 18, not: %d", m);
	char* res = (char*)malloc(sizeof(char) * 19);
	strncpy(res, msg->buffer+n+1, m);
	res[m] = '\0';
	cr_assert(strcmp("then a second line", res) == 0, "not as expected:'%s'", res);
}*/

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

