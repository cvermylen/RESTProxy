#include "http_headers.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <str_stack.h>

int decode_body_length(char* value, int field_length)
{
	int res = 0;
	int exp = field_length -1;
	for(int i = 0; i < field_length; res += (pow(10, exp--)) * (value[i] - 48), i++);
	return res;
}

void http_headers_init(http_header_t* header)
{
	for (int i=0; i < NUM_HTTP_HEADERS; i++) {
		header->headers[i] = stack_init();
	}
}

void decode_http_headers_init(http_header_t* header, char* buffer, int data_len)
{
	header->buff = buffer;
	header->cur_loc = 0;
	header->last_semicolon = -1;
	header->max_len = data_len;
}

/*
 \pre buff is not NULL
 \pre start_of_line <= cur_loc <= max_len
*/
void skip_eol(http_header_t* header)
{
	if (header->cur_loc < header->max_len && header->buff[header->cur_loc] == 0x0A
			&& header->buff[header->cur_loc + 1] == 0x0D){
		header->cur_loc += 2;
	}else if(header->buff[header->cur_loc] == '\n'){
		header->cur_loc += 1;
	}
	header->start_of_line = header-> cur_loc;
}

int is_eol_reached(http_header_t* header)
{
	return (header->cur_loc == header->max_len) || 
		((header->cur_loc < header->max_len) && 
			 (header->cur_loc < header->max_len && 
				header->buff[header->cur_loc] == 0x0A && 
				header->buff[header->cur_loc+1] == 0x0D) ||
			(header->buff[header->cur_loc] == '\n' ));
}

http_header_t* get_next_line(http_header_t* header)
{
	skip_eol(header);
	while(!is_eol_reached(header)) {
		if(header->buff[header->cur_loc] == ':')
			header->last_semicolon = header->cur_loc;
		header->cur_loc +=1;
	}
	return header;
}

int header_strlen(http_header_t* header)
{
	return header->cur_loc - header->start_of_line;
}

char* strmncpy(char* buffer, int start, int end)
{
	char* result = (char*)malloc(sizeof(char) * (end - start +1));
	strncpy(result, &buffer[start], end - start);
	result[end - start] = '\0';
	return result;
}

void http_headers_add(http_header_t* header)
{
	char* key = strmncpy(header->buff, header->start_of_line, header->last_semicolon);
	int index = find_header_index(key);
	int start_value_pos = header->last_semicolon + 1;
	while(header->buff[start_value_pos] == 0x20) start_value_pos++;
	char* value = strmncpy(header->buff, start_value_pos, header->cur_loc);
	if(index >= 0)
		str_stack_push(header->headers[index], value);
	free(key);
	free(value);
}

	
stack_head_t* http_headers_get(http_header_t* header, const int prop_key)
{
        stack_head_t* result = NULL;
        if(prop_key >= 0 && prop_key < NUM_HTTP_HEADERS){
                result = header->headers[prop_key];
        }
        return result;
}

void decode_http_headers(http_header_t* header)
{
	int cont = 1;
	get_next_line(header);
	while(cont && header_strlen(get_next_line(header)) > 0){
		if(header->last_semicolon > header->start_of_line){
			http_headers_add(header);
		}else{
			cont = 0;
		}
	}
}

void http_headers_free(http_header_t* header)
{
	for (int i=0; i < NUM_HTTP_HEADERS; i++) {
		str_stack_free(header->headers[i]);
	}
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
