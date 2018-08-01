#include "http_headers.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <str_stack.h>

char* http_header_buff;
int http_header_cur_loc;
int http_header_max_len;

int find_semicolon2(char* str, int limit)
{
	int i = 0;
	if(str == NULL)return -1;
	while(str[i] != ':' && i < limit && str[i] != '\0') i++;
	if(str[i] == ':')
		return i;
	else
		return -1;
}

http_header_line_t* decode_http_header_line(char* start_of_header, int line_length)
{
printf("Http header line: '%s'\n", start_of_header);
	http_header_line_t* res = NULL;
	if(line_length == 1 && start_of_header[0] == '\n'){
		res = (http_header_line_t*)malloc(sizeof(http_header_line_t));
		res->key = -1;
		return res;
	}
	int pos = find_semicolon2(start_of_header, line_length);
	if(pos > 0){
		res = (http_header_line_t*)malloc(sizeof(http_header_line_t));
		start_of_header[pos] = '\0';
		res->key = find_header_index(start_of_header);
		start_of_header[pos] = ':';
		for(pos++; start_of_header[pos] == ' '; pos++);
		res->value = start_of_header + (sizeof(char) * (pos));
		res->value_length = line_length - pos;
	}
	return res;
}

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

void decode_http_headers_init(char* buffer, int data_len)
{
	http_header_buff = buffer;
	http_header_cur_loc = 0;
	http_header_max_len = data_len;
}

char* get_next_line()
{
	int eol_clean = 0;
	while((http_header_buff[http_header_cur_loc] == 0x0A || http_header_buff[http_header_cur_loc] == 0x0D) && eol_clean < 2 ){
		eol_clean++;
		http_header_cur_loc++;
	}
	int start = http_header_cur_loc;
	while((http_header_buff[++http_header_cur_loc] != 0x0A && http_header_buff[http_header_cur_loc] != 0x0D) && http_header_cur_loc < http_header_max_len);
printf("string: cur loc:%d, max_len:%d, start:%d\n", http_header_cur_loc, http_header_max_len, start);
	char* result = NULL;
	if(http_header_cur_loc <= http_header_max_len){
		result = (char*)malloc(sizeof(char) * (http_header_cur_loc - start + 1));
		int i = 0;
		for(; i < http_header_cur_loc - start; result[i]=http_header_buff[start+i], i++);
printf("i:%d\n", i);
		result[i] = '\0';
		http_header_cur_loc += 1;
	}else{
		result = (char*) malloc (sizeof(char));
		result[0] = '\0';
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
printf("get_key_value_pair: %s\n", raw_string);
	char** kv = NULL;
	int start = 0;
	int mid = find_semicolon(raw_string);
	if(mid > 0){
		kv = (char**)malloc(sizeof(char*) * 2);
		kv[0] = NULL; kv[1] = NULL;
		kv[0] = (char*)malloc(sizeof(char) * mid);
		strncpy(kv[0], raw_string, mid);
		kv[0][mid] = '\0';
		int last = strlen(raw_string);
		if(last > (mid + 1)){
			mid = mid +1;
			while(isspace(raw_string[mid])) mid++;
			kv[1] = (char*)malloc(sizeof(char) * (last-mid+1));
			int i = 0;
			for(; i < (last-mid); kv[1][i] = raw_string[mid+ i], i++);
			kv[1][i] = '\0';
		}
	}
printf("done getKV\n");
	return kv;
}

void http_headers_add(http_header_t* header, char* key, char* value)
{
printf("Add: %s, '%s'\n", key, value);
	int index = find_header_index(key);
	str_stack_push(header->headers[index], value);
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
	char* line;
	int cont = 1;
	while (cont && strlen(line = get_next_line()) > 0){
printf("line: %d, '%x'\n", strlen(line), line[0]);
		char** prop = get_key_value_pair(line);
		if(prop != NULL){
			if(prop[0] != NULL){
printf("In between:'%s'\n", prop[1]);
				http_headers_add(header, prop[0], prop[1]);
printf("Before free prop 0\n");
				free(prop[0]);
printf("Before free prop 1\n");
				free(prop[1]);
			}
printf("Before free prop\n");
			free(prop);
		}
		cont = 0;
printf("Before free line\n");
		free(line);
	}
printf("Done decode\n");
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
