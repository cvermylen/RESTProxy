#include <stdlib.h>
#include <stdio.h>

#include "../buffers/shared_buffers.h"
#include "http_request.h"
#include "http_first_line.h"

request_t* new_http_request (int(*open_connection) (void* conn_params),
                             int(*feeder)(),
                             int (*send_data) (void* conn_params, char* dest_buffer, int max_buffer_size),
                             int(*close_connection) (void* conn_params),
                             void* connection_params)
{
    request_t *request = (request_t *) malloc(sizeof(request_t));
    request->buffer_size = TX_BUFFER_SIZE;
    request->http_message = new_http_message(feeder, connection_params, TX_BUFFER_SIZE);
    request->connection_params = connection_params;
    request->open_connection = open_connection;
    request->send_data = send_data;
    request->close_connection = close_connection;
    return request;
}

void receive_new_request_from_client (request_t *request)
{
    int fd = request->open_connection (request->connection_params);
    //REFACTOR
    if (fd < 0) {printf("Error not handled\n"); exit(0);}
    //END REFACTOR
    int bytes_read = read_next_buffer_from_source (request->http_message);
    request->type = http_message_decode_request_type (request->http_message);
    read_next_buffer_from_source (request->http_message);
    decode_http_message_header (request->http_message);
}

void process_request_message_body (request_t* request)
{
    switch (request->type) {
        case HTTP_REQUEST_GET:
            break;
        case HTTP_REQUEST_POST:
            break;
    }
}

int get_request_connection_keep_alive (request_t* request)
{
    return decode_keep_alive (request->http_message->header);
}

void close_client_connection (request_t* request)
{
    request->close_connection (request->connection_params);
}

request_t* release_request (request_t *d)
{
    http_message_free (d->http_message);
    free (d);
    return NULL;
}
