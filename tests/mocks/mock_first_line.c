#include "../../src/http/http_first_line.h"


int mock_called_http_decode_request_type;
int mock_result_http_decode_request_type;
int http_decode_request_type(int buffer_no, int length) {
    mock_called_http_decode_request_type += 1;
    return mock_result_http_decode_request_type;
}

int mock_http_decode_response_type;
int mock_result_http_decode_response_type;
int http_decode_response_type(int buffer_no, int length) {
    mock_http_decode_response_type += 1;
    return mock_result_http_decode_response_type;
}
