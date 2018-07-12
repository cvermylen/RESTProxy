#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

#include <stack.h>
#include "http_headers.h"

typedef struct {
	http_header_t header;
	char* body;
} http_message_t;

http_message_t* http_message_init();

void http_message_free(http_message_t* msg);

stack_head_t* http_message_get_header(http_message_t* msg, const int prop_key);

void http_message_add_header(http_message_t* msg, const int prop_key, char* value);

http_message_t* parse(char* raw_message, const int message_length);

#endif
