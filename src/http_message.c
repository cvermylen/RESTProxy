#include "http_message.h"
#include "request_reply.h"
#include "http_headers.h"
#include "shared_buffers.h"
#include <str_stack.h>
#include <stdlib.h>

http_message_t* http_message_init(int buff_no, char* buffer, int size)
{
	http_message_t* http_message = (http_message_t*)malloc(sizeof(http_message_t));
	http_headers_init(&(http_message->header));
	http_message->buffer_no = buff_no;
	http_message->buffer = buffer;
	http_message->raw_message_length = size;
	http_message->current_parsing_ptr = get_line_length(buffer); //skip first line as already processed
	return http_message;
}

char* http_message_get_request_buffer(http_message_t* msg)
{
	return msg->buffer;
}

//int calculate_next_header_line(http_message_t* msg, http_header_line_t* line)
//{
	
//}

void http_message_update_status(http_message_t* msg, int start_pos, int length)
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
		}
		break;
	case HTTP_MSG_STATUS_HEADER:
printf("HTTP_MSG_STATUS_HEADER:%d, %d\n", start_pos, length);
		//decode a header line
		header_line = decode_http_header_line(&msg->buffer[sizeof(char) * start_pos], length);
		if(header_line) {
			//calculate_next_header_line(msg, header_line);
			switch(header_line->key){
			case HTTP_CONTENT_LENGTH:
printf("HTTP_CONTENT_LENGTH\n");
				msg->body_length = decode_body_length(header_line->value, header_line->value_length);
				break;
			case -1:
				msg->status = HTTP_MSG_STATUS_HEADER_COMPLETE;
printf("End of header\n");
				break;
			default:
				break;
			}
		}else{
			printf("Expected a valid header line");
			exit(0);
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
}

int get_line_length(char* buffer)
{
	int n = 0;
	while(buffer[n] != '\n' && buffer[n] != '\0')
		n++;
	return n;
}

int read_from_buffer(int fd, http_message_t* msg, int start_index)
{
printf("read_from_buffer start:%d\n", start_index);
	if(start_index == msg->raw_message_length){
		msg->raw_message_length = read_from_socket(fd, msg->buffer + (start_index*sizeof(char)), TX_BUFFER_SIZE);
	}
	int n = get_line_length(msg->buffer + (start_index * sizeof(char)));
	return n;
}

http_message_t* http_message_read_from_socket(int fd, http_message_t* msg)
{
	int n, pos = 0;
	do{
		n = read_from_buffer(fd, msg, pos);
		http_message_update_status(msg, pos, n);
		pos += n + 1;
	} while(msg->status != HTTP_MSG_STATUS_BODY_COMPLETE);
	return msg;
}

void read_full_http_GET_request(int fd, http_message_t* msg)
{
printf("read_full_http_GET_request\n");
	int n, pos;
	do {
		n = read_from_buffer(fd, msg, pos);
		http_message_update_status(msg, pos, n);
	} while(msg->status != HTTP_MSG_STATUS_HEADER_COMPLETE);
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
