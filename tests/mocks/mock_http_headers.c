#include "../../src/http/http_headers.h"

int mock_http_headers_free;
void http_headers_free(http_header_t *header) {
    mock_http_headers_free = 1;
}

int mock_called_decode_http_headers;
int mock_result_decode_http_headers;
int decode_http_headers(http_header_t *header) {
    mock_called_decode_http_headers += 1;
    return mock_result_decode_http_headers;
}

int mock_called_decode_body_length;
int mock_result_decode_body_length;
int decode_body_length(http_header_t *header) {
    mock_called_decode_body_length += 1;
    return mock_result_decode_body_length;
}
