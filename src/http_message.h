#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

#include <stack.h>
#include "http_headers.h"

#define HTTP_MSG_STATUS_INIT	0
#define HTTP_MSG_STATUS_HEADER	1
#define HTTP_MSG_STATUS_HEADER_COMPLETE	2
#define HTTP_MSG_STATUS_BODY_INCOMPLETE	4
#define HTTP_MSG_STATUS_BODY_COMPLETE	8

typedef struct {
	int function;	//GET, POST
	char status;
	char* buffer;
	int buffer_no;
	int raw_message_length;
	http_header_t header;
	int body_length;
	char* body;
	int current_parsing_ptr;
} http_message_t;

http_message_t* http_message_init();

int get_line_length(char* buffer);

char* http_message_get_request_buffer(http_message_t* msg);

int read_from_buffer(int fd, http_message_t* msg, int start_index);

void http_message_free(http_message_t* msg);

void decode_http_message(http_message_t* msg);

http_message_t* parse(char* raw_message, const int message_length);

#endif
