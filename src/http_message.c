#include "http_message.h"
#include "http_headers.h"
#include <str_stack.h>
#include <stdlib.h>

http_message_t* http_message_init()
{
	http_message_t* http_message = (http_message_t*)malloc(sizeof(http_message_t));
	for(int i=0; i < NUM_HTTP_HEADERS; i++){
		http_message->header.headers[i] = stack_init();
	}
	return http_message;
}

void http_message_free(http_message_t* msg)
{
	for(int i=0; i < NUM_HTTP_HEADERS; i++){
		str_stack_free(msg->header.headers[i]);
	}
	free(msg);
}

stack_head_t* http_message_get_header(http_message_t* msg, const int prop_key)
{
	stack_head_t* result = NULL;
	if(prop_key >= 0 && prop_key < NUM_HTTP_HEADERS){
		result = msg->header.headers[prop_key];
	}
	return result;
}

void http_message_add_header(http_message_t* msg, const int prop_key, char* value)
{
	str_stack_push(msg->header.headers[prop_key], value);
}
