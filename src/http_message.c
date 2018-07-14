#include "http_message.h"
#include "http_headers.h"
#include <str_stack.h>
#include <stdlib.h>

http_message_t* http_message_init()
{
	http_message_t* http_message = (http_message_t*)malloc(sizeof(http_message_t));
	http_headers_init(&(http_message->header));
	return http_message;
}

void http_message_free(http_message_t* msg)
{
	http_headers_free(&(msg->header));
	free(msg);
}
