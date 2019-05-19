#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

//#include <stack.h>
#include "../frame_buffers/circular_buffer.h"
#include "http_headers.h"

#define HTTP_MSG_STATUS_INIT	0
#define HTTP_MSG_STATUS_HEADER	1              //Message cannot be re released to destination until header has been fully received
#define HTTP_MSG_STATUS_HEADER_COMPLETE	2      //Now message can be released
#define HTTP_MSG_STATUS_BODY_INCOMPLETE	4
#define HTTP_MSG_STATUS_BODY_COMPLETE	8

#define BUFFER_SIZE         4
typedef struct {
    int fd; //Message coming from, maybe could be replaced with a callback that would take as parameter the buffer and the max size.
	int function;	//GET, POST  or 3 digit for response. TODO: REFACTOR and include this in either request for get/post or reply.
	char status;
	circular_buffer_t* buffers;
	int raw_message_length;
	http_header_t* header;
	int body_length;
	char* body; // Messages can be > 4Gb...
} http_message_t;

/*! Next refactoring: replace the 'fd' parameter by a callback function
 *
 * @param fd
 */
http_message_t* receive_new_http_message(int fd);

void http_message_decode_request_type (http_message_t* msg);

/*! Read the response code (200, 40x...)
 * @param msg
 */
void http_message_decode_response_type(http_message_t* msg);

/*!
 * \private Called from receive_new_http_message.
 * @param fd
 * @return
 */
http_message_t* http_message_init(int fd);

int read_next_buffer_from_source (http_message_t* msg);

/*!
 * Liberates the buffer pointed by 'last_sent', supposedly it has been sent.
 * @param msg
 * @param buffno
 */
void http_message_free_buffer (http_message_t* msg);

int get_line_length(char* buffer);

int read_from_buffer(int fd, http_message_t* msg, int start_index);

void http_message_free(http_message_t* msg);

int decode_http_message_header(int fd, http_message_t* msg);

void receive_body(int fd, http_message_t* msg, int start_pos);

void decode_http_message(http_message_t* msg);

http_message_t* parse(char* raw_message, const int message_length);

void send_next_buffer_to_destination (http_message_t* msg, char move_pointer, int destination_fd);
#endif
