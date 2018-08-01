#ifndef HTTP_FIRST_LINE
#define HTTP_FIRST_LINE

#define HTTP_REQUEST_GET	1
#define HTTP_REQUEST_POST	2

#define HTTP_RESPONSE_200	200
#define HTTP_RESPONSE_404	404

int http_decode_request_type(char* buffer, int length);

int http_decode_response_type(char* buffer, int length);

#endif
