#include "http_message.h"
#include "request_reply.h"
#include "http_headers.h"
#include "shared_buffers.h"
#include <str_stack.h>
#include <stdlib.h>

http_message_t* http_message_init()
{
	http_message_t* http_message = (http_message_t*)malloc(sizeof(http_message_t));
	http_headers_init(&(http_message->header));
	http_message->buffer_no = alloc_buffer();
	http_message->buffer = get_buffer(http_message->buffer_no);
	return http_message;
}

char* http_message_get_request_buffer(http_message_t* msg)
{
	return msg->buffer;
}

void decode_http_message(http_message_t* msg)
{
	decode_http_headers_init(msg->buffer, msg->raw_message_length);
	decode_http_headers(&(msg->header));
}

void http_message_free(http_message_t* msg)
{
	http_headers_free(&(msg->header));
	free(msg);
}
