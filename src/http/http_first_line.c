#include "http_first_line.h"
#include <string.h>
#include <stdio.h>

int http_decode_request_type(char* buffer, int length)
{
        if(strncmp(buffer, "GET", 3) == 0)
                return HTTP_REQUEST_GET;
        if(strncmp(buffer, "POST", 4) == 0)
                return HTTP_REQUEST_POST;
	return 0;
}

//TODO: use str2int from the header module
int http_decode_response_type(char* buffer, int length)
{
	int res = 0;
	if(strncmp(buffer, "HTTP", 4) == 0){
		int i = 0;
		while(buffer[i++] != 0x20);
		while(buffer[i++] == 0x20);
		char* code = &(buffer[i]);
		for(int j=i-1; j<i+2;j++){
			res = res * 10;
			res += buffer[j] - 48;
		}
	}
printf("http_decode_response:%d\n", res);
	return res;
}
