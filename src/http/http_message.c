#include "http_message.h"
#include "../request_reply.h"
#include "http_headers.h"
#include "../buffers/shared_buffers.h"
#include "../socket/socket_connector.h"
#include "http_first_line.h"
#include <stdlib.h>

unsigned int http_message_buffer_size = BUFFER_SIZE;

http_message_t* new_http_message ((*feeder)(), void* connection_params, int buffer_size)
{
    http_message_t* http_message = NULL;
    if (http_message_buffer_size > 0) {
        http_message = (http_message_t *) malloc(sizeof(http_message_t));
        http_message->status = HTTP_MSG_STATUS_INIT;
        http_message->buffers = new_circular_buffer(http_message_buffer_size, feeder, connection_params, buffer_size);
        http_message->header = http_headers_init(http_message->buffers);
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
int read_next_buffer_from_source (http_message_t* msg)
{
    int buffer_no = alloc_entry_in_circular_buffer (msg->buffers);
    int r = 0;
    if (buffer_no >= 0) {
        r = read_into_next_buffer (msg->buffers);
        set_data_size_for_last_received_buffer (msg->buffers, r);
    }
    return r;
}

int http_message_decode_request_type (http_message_t* msg)
{
    return http_decode_request_type(get_last_received_buffer(msg->buffers), get_last_received_size(msg->buffers));
}

int http_message_decode_response_type(http_message_t* msg)
{
    return http_decode_response_type(get_last_received_buffer(msg->buffers), get_last_received_size(msg->buffers));
}

void decode_http_message_header(http_message_t *msg) {
    msg->status = HTTP_MSG_STATUS_HEADER;
    decode_http_headers(msg->header);
    msg->raw_message_length = msg->header->header_size;
    msg->body_length = decode_body_length(msg->header);
    msg->status = HTTP_MSG_STATUS_HEADER_COMPLETE;
}

/*!
 * Read as much data as we can fit in the available buffers, otherwise go to sleep until some are freed.
 * Operation will start at 'cur_loc' until no data is available on the stream
 * @param fd
 * @param msg
 * @param start_pos
 */
void http_message_receive_body (http_message_t* msg)
{
    msg->status = HTTP_MSG_STATUS_BODY_INCOMPLETE;
    int n = 0;
    do {
        msg->raw_message_length += n;
        n = read_next_buffer_from_source (msg);
    } while (n > 0);
    msg->status = HTTP_MSG_STATUS_BODY_COMPLETE;
}

/*!
 * Send the next buffer to its destination, and possibly indicates the buffer as 'sent'.
 * @param msg
 */
void send_next_buffer_to_destination (int(*send_data)(), void* connection_params, http_message_t* msg, char move_pointer)
{
    if (!is_empty_circular_buffer(msg->buffers)) {
        char* buffer = get_to_be_sent_buffer(msg->buffers);
        send_data(connection_params, buffer, get_to_be_sent_size(msg->buffers));
        if (move_pointer) {
            buffer_has_been_sent (msg->buffers);
        }
    }
}

void http_message_free(http_message_t *msg) {
    http_headers_free(msg->header);
    free_circular_buffer (msg->buffers);
    free(msg);
}
