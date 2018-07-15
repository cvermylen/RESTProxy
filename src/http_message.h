#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

#include <stack.h>
#include "http_headers.h"

typedef struct {
	char* buffer;
	int buffer_no;
	int raw_message_length;
	http_header_t header;
	char* body;
} http_message_t;

http_message_t* http_message_init();

char* http_message_get_request_buffer(http_message_t* msg);

void http_message_free(http_message_t* msg);

void decode_http_message(http_message_t* msg);

http_message_t* parse(char* raw_message, const int message_length);

#endif
