#include "http_headers.h"
#include <stdlib.h>
#include <string.h>
#include <str_stack.h>

char* http_header_buff;
int http_header_cur_loc;
int http_header_max_len;

void decode_http_headers_init(request_t* request)
{
	http_header_buff = request->buffer;
	http_header_cur_loc = 0;
	http_header_max_len = request->data_len;
}

char* get_next_line()
{
	int start = http_header_cur_loc;
	while((http_header_buff[++http_header_cur_loc] != '\n') && http_header_cur_loc < http_header_max_len);
printf("string:%s cur loc:%d, max_len:%d, start:%d\n", http_header_buff,http_header_cur_loc, http_header_max_len, start);
	char* result = NULL;
	if(http_header_cur_loc <= http_header_max_len){
		result = (char*)malloc(sizeof(char) * (http_header_cur_loc - start + 1));
		int i = 0;
		for(; i < http_header_cur_loc - start; result[i]=http_header_buff[start+i], i++);
printf("i:%d\n", i);
		result[start+i] = '\0';
		http_header_cur_loc += 1;
	}
	return result;
}

int find_semicolon(char* str)
{
	int i = 0;
	if(str == NULL)return -1;
	while(str[i] != ':' && str[i] != '\0') i++;
	if(str[i] == ':')
		return i;
	else
		return -1;
}

char** get_key_value_pair(char* raw_string)
{
	char** kv = (char**)malloc(sizeof(char**) * 2);
	kv[0] = NULL; kv[1] = NULL;
	int start = 0;
	int mid = find_semicolon(raw_string);
	if(mid > 0){
		kv[0] = (char*)malloc(sizeof(char) * mid);
		strncpy(kv[0], raw_string, mid);
		kv[0][mid] = '\0';
		int last = strlen(raw_string);
		if(last > (mid + 1)){
			mid = mid +1;
			kv[1] = (char*)malloc(sizeof(char) * (last-mid+1));
			int i = 0;
			for(; i < (last-mid); kv[1][i] = raw_string[mid+ i], i++);
			kv[1][i] = '\0';
		}
	}
	return kv;
}

void put_http_header(http_header_t* header, char* key, char* value)
{
	int index = find_header_index(key);
	str_stack_push(header->headers[index], value);
}

void decode_http_headers(http_header_t* header, request_t* request)
{
	decode_http_headers_init(request);
	char* line;
	while (strlen(line = get_next_line()) > 0){
		char** prop = get_key_value_pair(line);
		put_http_header(header, prop[0], prop[1]);
	}
}

void calculate_http_transition(request_t* request)
{

}

int find_header_index(const char* header)
{
	size_t i = NUM_HTTP_HEADERS / 2;
	int length = NUM_HTTP_HEADERS;
	while(i < length){
		int comparison = strcmp(HTTP_HEADER_STRINGS[i], header);
        	if (comparison == 0) {
            		return i;
        	}
	        if (comparison < 0) {
	            i += (length - i + 1) / 2;
       		} else {
	            length = i;
       		    i /= 2;
        	}
    	}
    	return -1;
}

char* HTTP_HEADER_STRINGS[] = {
"Accept",
"Accept-Charset",
"Accept-Encoding",
"Accept-Language",
"Accept-Ranges",
"Access-Control-Allow-Credentials",
"Access-Control-Allow-Headers",
"Access-Control-Allow-Methods",
"Access-Control-Allow-Origin",
"Access-Control-Expose-Headers",
"Access-Control-Max-Age",
"Access-Control-Request-Headers",
"Access-Control-Request-Method",
"Age",
"Allow",
"Authorization",
"Cache-Control",
"Connection",
"Content-Disposition",
"Content-Encoding",
"Content-Language",
"Content-Length",
"Content-Location",
"Content-Range",
"Content-Security-Policy",
"Content-Security-Policy-Report-Only",
"Content-Type",
"Cookie",
"Cookie2",
"DNT",
"Date",
"ETag",
"Expect",
"Expect-CT",
"Expires",
"Forwarded",
"From",
"Host",
"If-Match",
"If-Modified-Since",
"If-None-Match",
"If-Range",
"If-Unmodified-Since",
"Keep-Alive",
"Large-Allocation",
"Last-Modified",
"Location",
"Origin",
"Pragma",
"Proxy-Authenticate",
"Proxy-Authorization",
"Public-Key-Pins",
"Public-Key-Pins-Report-Only",
"Range",
"Referer",
"Referrer-Policy",
"Retry-After",
"Server",
"Server-Timing",
"Set-Cookie",
"Set-Cookie2",
"SourceMap",
"Strict-Transport-Security",
"TE",
"Timing-Allow-Origin",
"Tk",
"Trailer",
"Transfer-Encoding",
"Upgrade-Insecure-Requests",
"User-Agent",
"Vary",
"Via",
"WWW-Authenticate",
"Warning",
"X-Content-Type-Options",
"X-DNS-Prefetch-Control",
"X-Forwarded-For",
"X-Forwarded-Host",
"X-Forwarded-Proto",
"X-Frame-Options",
"X-XSS-Protection"
};
