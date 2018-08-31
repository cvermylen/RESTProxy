#include "http_message.h"
#include "request_reply.h"
#include "http_headers.h"
#include "shared_buffers.h"
#include <str_stack.h>
#include <stdlib.h>

http_message_t* http_message_init(int buff_no, char* buffer, int code, int sz)
{
	http_message_t* http_message = (http_message_t*)malloc(sizeof(http_message_t));
	http_headers_init(&(http_message->header));
	http_message->buffer_no = buff_no;
	http_message->buffer = buffer;
	http_message->function = code;
        http_message->raw_message_length = sz;
	return http_message;
}

char* http_message_get_request_buffer(http_message_t* msg)
{
	return msg->buffer;
}

int decode_http_message_header(int fd, http_message_t* msg)
{
	decode_http_headers_init(&(msg->header), fd, msg->buffer, msg->raw_message_length);
	int end_of_header = decode_http_headers(&(msg->header));
	msg->raw_message_length = msg->header.max_len;
	msg->body_length = decode_body_length(&(msg->header));
	return end_of_header;
}

void receive_body(int fd, http_message_t* msg, int start_pos)
{
	int remain = (start_pos + msg->body_length + 1) - msg->raw_message_length;
printf("receive body: raw length:%d, start_pos:%d, remainder:%d\n", msg->raw_message_length, start_pos, remain);
	while(remain > 0){
		int n = read_from_socket(fd, &(msg->buffer[msg->raw_message_length]), TX_BUFFER_SIZE);
if(n==0)exit(0);
		remain -= n;
		msg->raw_message_length += n;
	}
}

void receive_reply(reply_t* reply)
{
	accept_reply_from_server(reply);
	int start_of_body = decode_http_message_header(reply->content.sock->fd, reply->response_message);
printf("Body length:%d\n", reply->response_message->body_length);
	receive_body(reply->content.sock->fd, reply->response_message, start_of_body);
printf("Body received\n");
}

void http_message_free(http_message_t* msg)
{
	http_headers_free(&(msg->header));
	free(msg);
}
