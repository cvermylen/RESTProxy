#ifndef HTTP_FIRST_LINE
#define HTTP_FIRST_LINE

#define HTTP_REQUEST_GET	1
#define HTTP_REQUEST_POST	2

int http_decode_request_type(char* buffer, int length);

#endif
