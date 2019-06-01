#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

//#include <stack.h>
#include "../buffers/circular_buffer.h"
#include "http_headers.h"


#define HTTP_MSG_STATUS_INIT	0
#define HTTP_MSG_STATUS_HEADER	1              //Message cannot be re released to destination until header has been fully received
#define HTTP_MSG_STATUS_HEADER_COMPLETE	2      //Now message can be released
#define HTTP_MSG_STATUS_BODY_INCOMPLETE	4
#define HTTP_MSG_STATUS_BODY_COMPLETE	8

#define BUFFER_SIZE         4
typedef struct {
    int fd; //Used by the feeder function
    int (*feeder)(int fd, char* buffer, int buffer_size); //Message coming from, maybe could be replaced with a callback that would take as parameter the buffer and the max size.
	char status;  //See #defines above
	circular_buffer_t* buffers;
	int raw_message_length;  //Total size of the http message, including the first line, the header and the body
	http_header_t* header;
	int body_length;   //Size of the http body part, as indicated by the header field 'body-length'
} http_message_t;

/*!
 * Called from receive_new_http_message.
 * @param fd
 * @return
 */
http_message_t* new_http_message(int fd, int (*feeder)(int fd, char* buffer, int buffer_size), int buffer_size);

/*! Next refactoring: replace the 'fd' parameter by a callback function
 *
 * @param fd
 */
http_message_t* receive_new_http_message(int fd, int (*feeder)(int fd, char* buffer, int buffer_size), int buffer_size);

/*!
 * @param msg
 * @return  GET or POST ...
 */
int http_message_decode_request_type (http_message_t* msg);

/*! Read the response code (200, 40x...)
 * @param msg
 */
int http_message_decode_response_type(http_message_t* msg);

int read_next_buffer_from_source (http_message_t* msg);

/*! Frees all resources allocated to this message
 * @param msg
 */
void http_message_free(http_message_t* msg);

/*! Initiate the reading and decoding of the HTTP HEADERs.
 * i.e. the message body length will be available after this call.
 * @param fd
 * @param msg
 */
void decode_http_message_header(http_message_t* msg);

void http_message_receive_body(http_message_t *msg);

void send_next_buffer_to_destination (http_message_t* msg, char move_pointer, int destination_fd);
#endif
