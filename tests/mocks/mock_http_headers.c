#include "../../src/http/http_headers.h"
#include <stdlib.h>

int mock_called_http_headers_init;
http_header_t* http_headers_init(circular_buffer_t* buffers) {
    http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
    mock_called_http_headers_init += 1;
    return header;
}

int mock_called_http_headers_free;
void http_headers_free(http_header_t *header) {
    mock_called_http_headers_free += 1;
}

int mock_called_decode_http_headers;
void decode_http_headers(http_header_t *header) {
    mock_called_decode_http_headers += 1;
}

int mock_called_decode_body_length;
int mock_result_decode_body_length;
int decode_body_length(http_header_t *header) {
    mock_called_decode_body_length += 1;
    return mock_result_decode_body_length;
}

int mock_called_decode_keep_alive;
int mock_return_decode_keep_alive;
int decode_keep_alive (http_header_t* header)
{
    mock_called_decode_keep_alive += 1;
    return mock_return_decode_keep_alive;
}