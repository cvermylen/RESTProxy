#include "http_first_line.h"

int http_decode_request_type(char* buffer, int length)
{
        if(strncmp(buffer, "GET", 3) == 0)
                return HTTP_REQUEST_GET;
        if(strncmp(buffer, "POST", 4) == 0)
                return HTTP_REQUEST_POST;
	return 0;
}
