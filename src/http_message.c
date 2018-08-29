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
	int remain = msg->raw_message_length - (start_pos + msg->body_length + 1);
printf("receive body: raw length:%d, start_pos:%d, remainder:%d\n", msg->raw_message_length, start_pos, remain);
	while(remain > 0){
		int n = read_from_socket(fd, &(msg->buffer[start_pos + msg->body_length - remain]), TX_BUFFER_SIZE);
if(n==0)exit(0);
		remain -= n;
	}
}

void receive_reply(reply_t* reply)
{
	int buff_no = alloc_buffer();
	char* buffer = get_buffer(buff_no);
	//read first line
	int sz = read_from_socket(reply->content.sock->fd, buffer, TX_BUFFER_SIZE);
	int code = http_decode_response_type(buffer, sz);
	//int n = split_receive(reply->content.sock->fd, buffer, TX_BUFFER_SIZE);
printf("RECEIVED:%d response:%s\n", sz, buffer);
	//int code = http_decode_response_type(buffer, n);
	reply->response_message = http_message_init(buff_no, buffer, code, sz);
	int start_of_body = decode_http_message_header(reply->content.sock->fd, reply->response_message);
printf("Body length:%d\n", reply->response_message->body_length);
	receive_body(reply->content.sock->fd, reply->response_message, start_of_body);
printf("Body received\n");
}

/*void http_message_update_status(http_message_t* msg, int start_pos, int length)
{
	int op = 0;
	http_header_line_t* header_line = NULL;
	switch(msg->status){
	case HTTP_MSG_STATUS_INIT:
printf("HTTP_MSG_STATUS_INIT\n");
		//decode header line
		if(msg->function > 0) {
			msg->status = HTTP_MSG_STATUS_HEADER;
		}else{
			printf("Message is not  HTTP\n");
			exit(0); //TODO remove
		}
		break;
	case HTTP_MSG_STATUS_HEADER:
printf("HTTP_MSG_STATUS_HEADER:%d, %d\n", start_pos, length);
		//decode a header line
		if(!is_last_header_line(&msg->buffer[sizeof(char) * start_pos], length)) {
			header_line = decode_http_header_line(&msg->buffer[sizeof(char) * start_pos], length);
			//calculate_next_header_line(msg, header_line);
			switch(header_line->key){
			case HTTP_CONTENT_LENGTH:
printf("HTTP_CONTENT_LENGTH\n");
				msg->body_length = decode_body_length(header_line->value, header_line->value_length);
				break;
			default:
				break;
			}
		}else{
				msg->status = HTTP_MSG_STATUS_HEADER_COMPLETE;
		}
		break;
	case HTTP_MSG_STATUS_HEADER_COMPLETE:
printf("HTTP_MSG_STATUS_HEADER_COMPLETE\n");
		msg->body_length -= length - start_pos;
		if(msg->body_length == 0)
			msg->status = HTTP_MSG_STATUS_BODY_COMPLETE;
		//accumulate datao
		//previous state has to be HTTP_MSG_STATUS_LENGTH
		//calculate if complete
		break;
	}
}*/

void http_message_free(http_message_t* msg)
{
	http_headers_free(&(msg->header));
	free(msg);
}
