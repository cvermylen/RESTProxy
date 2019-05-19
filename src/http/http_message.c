#include "http_message.h"
#include "../request_reply.h"
#include "http_headers.h"
#include "../frame_buffers/shared_buffers.h"
#include "../socket/socket_connector.h"
#include "http_first_line.h"
#include <stdlib.h>

unsigned int http_message_buffer_size = BUFFER_SIZE;



//TODO NExt refactoring: remove the buffer from the parameters, and change the way the first buffer is read.
http_message_t* http_message_init(int fd) {
    http_message_t* http_message = NULL;
    if (http_message_buffer_size > 0) {
        http_message = (http_message_t *) malloc(sizeof(http_message_t));
        http_message->fd = fd;
        http_message->status = HTTP_MSG_STATUS_HEADER;
        http_message->header = NULL;
        http_message->buffers = new_circular_buffer(http_message_buffer_size);
        http_message->function = -1;
        http_message->raw_message_length = -1;
    } else {
        printf("ERROR: Messages should be allocated more than 1 buffer\n");
        exit(0);
    }
    return http_message;
}

/*! 1-1 = 0 => high = high + 1 mod 16
 *  10 - 15 = -5 => high = high + 1 mod 16
 *  15 - 1
 */
int read_next_buffer_from_source (http_message_t* msg) {
    int r = 0;
    if (((msg->last_received + 1) | http_message_buffer_size) != msg->last_sent) {
        msg->last_received = (msg->last_received + 1) & http_message_buffer_size;
        msg->buffers[msg->last_received] = alloc_buffer();
        r = read_from_socket(msg->fd, get_buffer(msg->buffers[msg->last_received]), TX_BUFFER_SIZE);
        msg->data_sizes[msg->last_received] = r;
    }
    return r;
}

http_message_t* receive_new_http_message(int fd)
{
    http_message_t* result = http_message_init (fd);
    if(result != NULL) {
        int r = read_next_buffer_from_source (result);
    }
    return result;
}

void http_message_decode_request_type (http_message_t* msg)
{
    int code = http_decode_request_type(get_last_received_buffer(msg), get_last_received_size(msg));
    msg->function = code;
}

void http_message_decode_response_type(http_message_t* msg){
    int code = http_decode_response_type(get_last_received_buffer(msg), get_last_received_size(msg));
    msg->function = code;
}

void http_message_free_buffer (http_message_t* msg)
{
    if (msg->last_sent != msg->last_received) {
        free_buffer(msg->buffers[msg->last_sent]);
        msg->buffers[msg->last_sent] = -1;
        msg->data_sizes[msg->last_sent] = -1;
        msg->last_sent = (msg->last_sent + 1) & http_message_buffer_size;
    }
}

int decode_http_message_header(int fd, http_message_t *msg) {
    //decode_http_headers_init(&(msg->header), fd, msg->buffer, msg->raw_message_length);
    int end_of_header = decode_http_headers(msg->header);
    msg->raw_message_length = msg->header->max_len;
    msg->body_length = decode_body_length(msg->header);
    return end_of_header;
}

/*!
 * Read as much data as we can fit in the available buffers, otherwise go to sleep until some are freed.
 * @param fd
 * @param msg
 * @param start_pos
 */
void receive_body(int fd, http_message_t *msg, int start_pos) {
    int remain = (start_pos + msg->body_length + 1) - msg->raw_message_length;
    printf("receive body: raw length:%d, start_pos:%d, remainder:%d\n", msg->raw_message_length, start_pos, remain);
    while (remain > 0) {
        int n = read_next_buffer_from_source(msg);
        if (n == 0)exit(0);
        remain -= n;
        msg->raw_message_length += n;
    }
}

void http_message_free(http_message_t *msg) {
    http_headers_free(msg->header);
    free(msg);
}
