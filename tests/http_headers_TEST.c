#include "../src/http/http_headers.h"

#include <criterion/criterion.h>

Test(strmncpy, empty)
{
	char* str = "a line";
	char* result = strmncpy(str, 2, 2);
	cr_assert(0 == strlen(result), "length not 0 as expected: %lu", strlen(result));
	cr_assert(strcmp("", result) == 0, "expected '', not: %s", result);
}

Test(strmncpy, happy)
{
	char* str = "a non-empty line";
	char* result = strmncpy(str, 2, 5);
	cr_assert(3 == strlen(result), "length not 3 as expected: %lu", strlen(result));
	cr_assert(strcmp("non", result) == 0, "expected 'non', not: %s", result);
}

Test(http_headers_add, happy)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	http_headers_init(header);
	header->buff = "Server:value\n";
	header->start_of_line = 0;
	header->last_semicolon = 6;
	header->cur_loc = 12;
	header->max_len = strlen(header->buff);
	http_headers_add(header);
	int size = stack_depth(header->headers[57]);
	cr_assert(1 == size, "expected 1 element at 57, not:%d", size);
}

Test(http_headers_add, non_existing)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	http_headers_init(header);
	header->buff = "funny:value";
	header->start_of_line = 0;
	header->last_semicolon = 5;
	header->cur_loc = 11;
	header->max_len = strlen(header->buff);
	http_headers_add(header);
}

Test(eol, zero)
{
	char str[] = "";
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 0;
	skip_eol(header);
	cr_assert(0 == header->start_of_line, "should not have been updated");
	cr_assert(0 == header->cur_loc, "should not have been updated");
	cr_assert(0 == header->max_len, "should not have been updated");
}

Test(eol, pos_1)
{
	char str[] = "\n";
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 1;
	skip_eol(header);
	cr_assert(1 == header->start_of_line, "start_of_line expected:1, found:%d", header->start_of_line);
	cr_assert(1 == header->cur_loc, "cur_loc expected:1, found: %d", header->cur_loc);
	cr_assert(1 == header->max_len, "max_len should not have been updated");
}

Test(eol, pos_2)
{
	char str[] = {0x0A, 0x0D};
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 2;
	skip_eol(header);
	cr_assert(2 == header->start_of_line, "start_of_line expected:2, found:%d", header->start_of_line);
	cr_assert(2 == header->cur_loc, "cur_loc expected:2, found: %d", header->cur_loc);
	cr_assert(2 == header->max_len, "max_len should not have been updated");
}

Test(eol, pos_3)
{
	char str[] = {0x0A, 0x20, 0x0D};
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 3;
	skip_eol(header);
	cr_assert(1 == header->start_of_line, "start_of_line expected:0, found:%d", header->start_of_line);
	cr_assert(1 == header->cur_loc, "cur_loc expected:0, found: %d", header->cur_loc);
	cr_assert(3 == header->max_len, "max_len should not have been updated");
}

Test(eol, neg_1)
{
	char str[] = "  \n";
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 3;
	skip_eol(header);
	cr_assert(0 == header->start_of_line, "start_of_line expected:2, found:%d", header->start_of_line);
	cr_assert(0 == header->cur_loc, "cur_loc expected:2, found: %d", header->cur_loc);
	cr_assert(3 == header->max_len, "max_len should not have been updated");
}

Test(is_eol_reached, empty)
{
	char str[] = "";
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 0;
	int res = is_eol_reached(header);
	cr_assert(0 != res, "expected:true, found:%d", res);
}

Test(is_eol_reached, true_1)
{
	char str[] = "\n";
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 1;
	int res = is_eol_reached(header);
	cr_assert(0 != res, "expected:true, found:%d", res);
}

Test(is_eol_reached, true_2)
{
	char str[] = {0x0A, 0x0D};;
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 2;
	int res = is_eol_reached(header);
	cr_assert(0 != res, "expected:true, found:%d", res);
}

Test(is_eol_reached, false_1)
{
	char str[] = " \n";
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 2;
	int res = is_eol_reached(header);
	cr_assert(0 == res, "expected:false, found:%d", res);
}

Test(is_eol_reached, false_2)
{
	char str[] = {0x0D, 0x020};
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 2;
	int res = is_eol_reached(header);
	cr_assert(0 == res, "expected:false, found:%d", res);
}

Test(get_next_line, semicolon_true)
{
	char str[]= "key:value";
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->last_semicolon = -1;
	header->max_len = strlen(str);
	get_next_line(header);
	cr_assert(3 == header->last_semicolon, "not the expected position for semicolon: %d", header->last_semicolon);
}

Test(get_next_line, semicolon_false)
{
	char str[]= "keyvalue";
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->last_semicolon = -1;
	header->max_len = strlen(str);
	get_next_line(header);
	cr_assert(-1 == header->last_semicolon, "not the expected position for semicolon: %d", header->last_semicolon);
}

Test(get_next_line, special_char)
{
    char str[] = "Content-Type: text/html; charset=UTF-8";
    http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
    header->buff = str;
    header->start_of_line = 0;
    header->cur_loc = 0;
    header->last_semicolon = -1;
    header->max_len = strlen(str);
    get_next_line(header);
    http_headers_add(header);
    cr_assert(12 == header->last_semicolon, "not the expected position for semicolon: %d", header->last_semicolon);
}
Test(str2int, happy)
{
	char str[] = "3451";
	int res = str2int(str, strlen(str));
	cr_assert(3451 == res, "expected value of 3451, not:%d", res);
}

Test(str2int, enpty)
{
	char str[] = "";
	int res = str2int(str, 0);
	cr_assert(0 == res, "expected to be 0, not:%d", res);
}

Test(body_length, empty)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	http_headers_init(header);
	int result = decode_body_length(header);
	cr_assert(0 == result, "expected 0 if field not present, not:%d", result);
}

Test(body_length, happy)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	http_headers_init(header);
	str_stack_push(header->headers[HTTP_CONTENT_LENGTH], "32");
	int result = decode_body_length(header);
	cr_assert(32 == result, "expected 32, not:%d", result);
}

Test(decode_http_headers, empty_0)
{
	char str[] = "";
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->last_semicolon = -1;
	header->cur_loc = 0;
	header->max_len = 26;
	http_headers_init(header);
	decode_http_headers(header);
}

Test(decode_http_headers, empty_1)
{
	char str[] = "HTTP 1/1 GET\n";
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->last_semicolon = -1;
	header->cur_loc = 0;
	header->max_len = 26;
	http_headers_init(header);
	decode_http_headers(header);
}

Test(decode_http_headers, happy_1)
{
	char str[] = "HTTP 1/1 GET\nAccept: Apple\nServer: localhost\n\n<html>\n";
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = str;
	header->start_of_line = 0;
	header->last_semicolon = -1;
	header->cur_loc = 0;
	header->max_len = 26;
	http_headers_init(header);
	decode_http_headers(header);
	int size = stack_depth(header->headers[0]);
	cr_assert(1 == size, "expected 'Accept' in headers");
	size = stack_depth(header->headers[57]);
	cr_assert(1 == size, "expected 'Server' in headers");
}

Test(get_next_line, 1_line)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = "1_line, a single line";
	header->cur_loc = 0;
	header->max_len = strlen(header->buff);
	header->start_of_line = 0;
	get_next_line(header);
	cr_assert(0 == header->start_of_line, "start_of_line should not have changed: %d", header->start_of_line);
	cr_assert(21 == header->cur_loc, "end of line not as expected: %d", header->cur_loc);
}

Test(get_next_line, 2_line)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
    decode_http_headers_init(header, 0, "line1\nline2", strlen("line1\nline2"));

	get_next_line(header);
	get_next_line(header);
	cr_assert(6 == header->start_of_line, "start_of_line should not have changed: %d", header->start_of_line);
	cr_assert(11 == header->cur_loc, "end of line not as expected: %d", header->cur_loc);
}

Test(get_next_line, last_line)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
    decode_http_headers_init(header, 0, "a single line", strlen("a single line"));

	get_next_line(header);
	get_next_line(header);
	cr_assert(13 == header->cur_loc, "cur_loc should point after the last caracter, not:%d", header->cur_loc);
	cr_assert(13 == header->start_of_line, "start_of_line should point after the last caracter, not:%d", header->start_of_line);

}

Test(get_next_line, 3_lines)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = "first line\nsecond line\nthird line";
	header->cur_loc = 0;
	header->start_of_line = 0;
	header->max_len = strlen(header->buff);
	get_next_line(header);
	get_next_line(header);
	get_next_line(header);
	cr_assert(33 == header->cur_loc, "expcted end of third line at 33, not %d", header->cur_loc);
	cr_assert(23 == header->start_of_line, "expcted end of third line at 33, not %d", header->start_of_line);
}

Test(strlen, 1_line)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = "a line";
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = strlen(header->buff);
	get_next_line(header);
	int i = header_strlen(header);
	cr_assert(6 == i, "Not the expected length: %d", i);
}

Test(strlen, 1_line_cr)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = "a line\n";
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = strlen(header->buff);
	get_next_line(header);
	int i = header_strlen(header);
	cr_assert(6 == i, "Not the expected length: %d", i);
}

Test(strlen, 1_line_cr_2)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	char str[] = {'a',' ','l','i','n','e', 0x0A, 0x0D};
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = strlen(header->buff);
	get_next_line(header);
	int i = header_strlen(header);
	cr_assert(6 == i, "Not the expected length: %d", i);
}

Test(strlen, 2_line_cr)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = "a line\nsecond line";
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 18;
	get_next_line(header);
	get_next_line(header);
	int i = header_strlen(header);
	cr_assert(11 == i, "Not the expected length: %d", i);
}

Test(strlen, 2_line_cr_2)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	char str[] = {'a',' ','l','i','n','e',0x0A,0x0D,'s','e','c','o','n','d',' ','l','i','n','e'};
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 19;
	get_next_line(header);
	get_next_line(header);
	int i = header_strlen(header);
	cr_assert(11 == i, "Not the expected length: %d", i);
}

Test(strlen, empty)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	header->buff = "a line\n\nsecond line";
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 19;
	get_next_line(header);
	get_next_line(header);
	int i = header_strlen(header);
	cr_assert(0 == i, "Not the expected length: %d", i);
}

Test(strlen, empty_2)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	char str[] = {'a',' ','l','i','n','e',0x0A,0x0D,0x0A,0x0D,'s','e','c','o','n','d',' ','l','i','n','e'};
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 0;
	header->max_len = 21;
	get_next_line(header);
	get_next_line(header);
	int i = header_strlen(header);
	cr_assert(0 == i, "Not the expected length: %d", i);
}

Test(http_headers, get_empty)
{
        http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	http_headers_init(header);
        stack_head_t* empty = http_headers_get(header, 3);
        cr_assert(is_stack_empty(empty), "expected to return NULL on prop not found");
        http_headers_free(header);
}

Test(http_header, get_1)
{
        http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	http_headers_init(header);
	char str[] = "Accept-Language:value1";
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 22;
	header->max_len = strlen(str);
	header->last_semicolon = 15;
        http_headers_add(header);
        stack_head_t* st = http_headers_get(header, 3);
        cr_assert(1 == stack_depth(st), "expected stack depth to be 1, not: %d", stack_depth(st));
        char* s = str_stack_pop(st);
        cr_assert(strcmp("value1", s) == 0, "expected 'value1', not:%s", s);
        cr_assert(is_stack_empty(st), "expected stack to be empty after poping value");
        free(s);
        str_stack_free(st);
        http_headers_free(header);
}

Test(http_header, get_2)
{
        http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	http_headers_init(header);
	char str[] = "Accept-Language:value1";
	header->buff = str;
	header->start_of_line = 0;
	header->cur_loc = 21;
	header->max_len = strlen(str);
	header->last_semicolon = 15;
        http_headers_add(header);
        http_headers_add(header);
        stack_head_t* st = http_headers_get(header, 3);
        cr_assert(2 == stack_depth(st), "expected stack depth to be 2, not: %d", stack_depth(st));
        str_stack_free(st);
        http_headers_free(header);
}

Test(decode_http_header, parse_retrieve)
{
	http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
	http_headers_init(header);
	char msg[] = "HTTP/1.1 200 OK\nDate: Mon, 23 May 2005 22:38:34 GMT\nContent-Type: text/html; charset=UTF-8\nContent-Length: 14\nLast-Modified: Wed, 08 Jan 2003 23:11:55 GMT\nServer: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\nETag: \"3f80f-1b6-3e1cb03b\"\nAccept-Ranges: bytes\n\n<html>\n</html>";

	decode_http_headers_init(header, 0, msg, strlen(msg));
	decode_http_headers(header);
	stack_head_t* vals = http_headers_get(header, HTTP_CONTENT_TYPE);
	cr_assert(vals->num_elems == 1, "expected to have 1 element for Content-Type, not: %d", vals->num_elems);
	char* value = str_stack_pop(vals);
	cr_assert(strcmp("text/html; charset=UTF-8", value) == 0, "expected value 'text/html; charset=UTF-8', not %s", value);
	free(value);
	http_headers_free(header);
}

Test(http_headers, constants_sorted)
{
	for(int i=0; i < 80; i++){
		cr_assert(strcmp(HTTP_HEADER_STRINGS[i], HTTP_HEADER_STRINGS[i+1]) < 0, "Not sorted at %d", i);
	}
}

Test(http_headers, search_1)
{
	int pos = find_header_index("Trailer");
	cr_assert(pos == HTTP_TRAILER, "'Trailer' not at expected position:%d", HTTP_TRAILER);
}

Test(http_headers, search_first)
{
	int pos = find_header_index("Accept");
	cr_assert(pos == HTTP_ACCEPT, "first element in array 'Accept' not at expected position:%d", pos);
}

Test(http_headers, search_before_first)
{
	int pos = find_header_index("A");
	cr_assert(pos < 0, "element before first in array not returning expected position:%d", pos);
}

Test(http_headers, search_last)
{
	int pos = find_header_index("X-XSS-Protection");
	cr_assert(pos == HTTP_X_XSS_PROTECTION, "last element in array 'X-XSS-Protection' not at expected position:%d", pos);
}

Test(http_headers, search_after_last)
{
	int pos = find_header_index("Z");
	cr_assert(pos < 0, "element after last in array not returning expected position:%d", pos);
}

Test(http_headers, search_not_found)
{
	int pos = find_header_index("MMM");
	cr_assert(pos < 0, "string 'MMM' should not be found, instead returned:%d", pos);
}

Test(http_headers, accept_charset)
{
	cr_assert(strcmp("Accept-Charset", HTTP_HEADER_STRINGS[HTTP_ACCEPT_CHARSET]) == 0,
		 "Expected 'Accept-Charset', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCEPT_CHARSET]);
}

Test(http_headers, accept)
{
	cr_assert(strcmp("Accept", HTTP_HEADER_STRINGS[HTTP_ACCEPT]) == 0, 
		"Expected 'Accept', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCEPT]);
}

Test(http_headers, accept_encoding)
{
	cr_assert(strcmp("Accept-Encoding", HTTP_HEADER_STRINGS[HTTP_ACCEPT_ENCODING]) == 0, 
		"Expected 'Accept-Encoding', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCEPT_ENCODING]);
}

Test(http_headers, accept_language)
{
	cr_assert(strcmp("Accept-Language", HTTP_HEADER_STRINGS[HTTP_ACCEPT_LANGUAGE]) == 0, 
		"Expected 'Accept-Language', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCEPT_LANGUAGE]);
}

Test(http_headers, accept_ranges)
{
	cr_assert(strcmp("Accept-Ranges", HTTP_HEADER_STRINGS[HTTP_ACCEPT_RANGES]) == 0, 
		"Expected 'Accept-Ranges', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCEPT_RANGES]);
}

Test(http_headers, access_control_allow_credentials)
{
	cr_assert(strcmp("Access-Control-Allow-Credentials", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_ALLOW_CREDENTIALS]) == 0, 
		"Expected 'Access-Control-Allow-Credentials', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_ALLOW_CREDENTIALS]);
}

Test(http_headers, access_control_allow_headers)
{
	cr_assert(strcmp("Access-Control-Allow-Headers", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_ALLOW_HEADERS]) == 0, 
		"Expected 'Access-Control-Allow-Headers', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_ALLOW_HEADERS]);
}

Test(http_headers, access_control_allow_methods)
{
	cr_assert(strcmp("Access-Control-Allow-Methods", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_ALLOW_METHODS]) == 0, 
		"Expected 'Access-Control-Allow-Methods', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_ALLOW_METHODS]);
}

Test(http_headers, access_control_allow_origin)
{
	cr_assert(strcmp("Access-Control-Allow-Origin", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_ALLOW_ORIGIN]) == 0, 
		"Expected 'Access-Control-Allow-Origin', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_ALLOW_ORIGIN]);
}

Test(http_headers, access_control_expose_headers)
{
	cr_assert(strcmp("Access-Control-Expose-Headers", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_EXPOSE_HEADERS]) == 0, 
		"Expected 'Access-Control-Expose-Headers', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_EXPOSE_HEADERS]);
}

Test(http_headers, access_control_max_age)
{
	cr_assert(strcmp("Access-Control-Max-Age", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_MAX_AGE]) == 0, 
		"Expected 'Access-Control-Max-Age', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_MAX_AGE]);
}

Test(http_headers, access_control_request_headers)
{
	cr_assert(strcmp("Access-Control-Request-Headers", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_REQUEST_HEADERS]) == 0, 
		"Expected 'Access-Control-Request-Headers', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_REQUEST_HEADERS]);
}

Test(http_headers, access_control_request_method)
{
	cr_assert(strcmp("Access-Control-Request-Method", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_REQUEST_METHOD]) == 0, 
		"Expected 'Access-Control-Request-Method', not:%s", HTTP_HEADER_STRINGS[HTTP_ACCESS_CONTROL_REQUEST_METHOD]);
}

Test(http_headers, age)
{
	cr_assert(strcmp("Age", HTTP_HEADER_STRINGS[HTTP_AGE]) == 0, 
		"Expected 'Age', not:%s", HTTP_HEADER_STRINGS[HTTP_AGE]);
}

Test(http_headers, allow)
{
	cr_assert(strcmp("Allow", HTTP_HEADER_STRINGS[HTTP_ALLOW]) == 0, 
		"Expected 'Allow', not:%s", HTTP_HEADER_STRINGS[HTTP_ALLOW]);
}

Test(http_headers, authorization)
{
	cr_assert(strcmp("Authorization", HTTP_HEADER_STRINGS[HTTP_AUTHORIZATION]) == 0, 
		"Expected 'Authorization', not:%s", HTTP_HEADER_STRINGS[HTTP_AUTHORIZATION]);
}

Test(http_headers, cache_control)
{
	cr_assert(strcmp("Cache-Control", HTTP_HEADER_STRINGS[HTTP_CACHE_CONTROL]) == 0, 
		"Expected 'Cache-Control', not:%s", HTTP_HEADER_STRINGS[HTTP_CACHE_CONTROL]);
}

Test(http_headers, connection)
{
	cr_assert(strcmp("Connection", HTTP_HEADER_STRINGS[HTTP_CONNECTION]) == 0, 
		"Expected 'Connection', not:%s", HTTP_HEADER_STRINGS[HTTP_CONNECTION]);
}

Test(http_headers, content_disposition)
{
	cr_assert(strcmp("Content-Disposition", HTTP_HEADER_STRINGS[HTTP_CONTENT_DISPOSITION]) == 0, 
		"Expected 'Content-Disposition', not:%s", HTTP_HEADER_STRINGS[HTTP_CONTENT_DISPOSITION]);
}

Test(http_headers, content_encoding)
{
	cr_assert(strcmp("Content-Encoding", HTTP_HEADER_STRINGS[HTTP_CONTENT_ENCODING]) == 0, 
		"Expected 'Content-Encoding', not:%s", HTTP_HEADER_STRINGS[HTTP_CONTENT_ENCODING]);
}

Test(http_headers, content_language)
{
	cr_assert(strcmp("Content-Language", HTTP_HEADER_STRINGS[HTTP_CONTENT_LANGUAGE]) == 0, 
		"Expected 'Content-Language', not:%s", HTTP_HEADER_STRINGS[HTTP_CONTENT_LANGUAGE]);
}

Test(http_headers, content_length)
{
	cr_assert(strcmp("Content-Length", HTTP_HEADER_STRINGS[HTTP_CONTENT_LENGTH]) == 0, 
		"Expected 'Content-Length', not:%s", HTTP_HEADER_STRINGS[HTTP_CONTENT_LENGTH]);
}

Test(http_headers, content_location)
{
	cr_assert(strcmp("Content-Location", HTTP_HEADER_STRINGS[HTTP_CONTENT_LOCATION]) == 0, 
		"Expected 'Content-Location', not:%s", HTTP_HEADER_STRINGS[HTTP_CONTENT_LOCATION]);
}

Test(http_headers, content_range)
{
	cr_assert(strcmp("Content-Range", HTTP_HEADER_STRINGS[HTTP_CONTENT_RANGE]) == 0, 
		"Expected 'Content-Range', not:%s", HTTP_HEADER_STRINGS[HTTP_CONTENT_RANGE]);
}

Test(http_headers, content_security_policy)
{
	cr_assert(strcmp("Content-Security-Policy", HTTP_HEADER_STRINGS[HTTP_CONTENT_SECURITY_POLICY]) == 0, 
		"Expected 'Content-Security-Policy', not:%s", HTTP_HEADER_STRINGS[HTTP_CONTENT_SECURITY_POLICY]);
}

Test(http_headers, content_security_policy_report_only)
{
	cr_assert(strcmp("Content-Security-Policy-Report-Only", HTTP_HEADER_STRINGS[HTTP_CONTENT_SECURITY_POLICY_REPORT_ONLY]) == 0, 
		"Expected 'Content-Security-Policy-Report-Only', not:%s", HTTP_HEADER_STRINGS[HTTP_CONTENT_SECURITY_POLICY_REPORT_ONLY]);
}

Test(http_headers, content_type)
{
	cr_assert(strcmp("Content-Type", HTTP_HEADER_STRINGS[HTTP_CONTENT_TYPE]) == 0, 
		"Expected 'Content-Type', not:%s", HTTP_HEADER_STRINGS[HTTP_CONTENT_TYPE]);
}

Test(http_headers, cookie)
{
	cr_assert(strcmp("Cookie", HTTP_HEADER_STRINGS[HTTP_COOKIE]) == 0, 
		"Expected 'Cookie', not:%s", HTTP_HEADER_STRINGS[HTTP_COOKIE]);
}

Test(http_headers, cookie2)
{
	cr_assert(strcmp("Cookie2", HTTP_HEADER_STRINGS[HTTP_COOKIE2]) == 0, 
		"Expected 'Cookie2', not:%s", HTTP_HEADER_STRINGS[HTTP_COOKIE2]);
}

Test(http_headers, DNT)
{
	cr_assert(strcmp("DNT", HTTP_HEADER_STRINGS[HTTP_DNT]) == 0, 
		"Expected 'DNT', not:%s", HTTP_HEADER_STRINGS[HTTP_DNT]);
}

Test(http_headers, date)
{
	cr_assert(strcmp("Date", HTTP_HEADER_STRINGS[HTTP_DATE]) == 0, 
		"Expected 'Date', not:%s", HTTP_HEADER_STRINGS[HTTP_DATE]);
}

Test(http_headers, ETag)
{
	cr_assert(strcmp("ETag", HTTP_HEADER_STRINGS[HTTP_ETAG]) == 0, 
		"Expected 'ETag', not:%s", HTTP_HEADER_STRINGS[HTTP_ETAG]);
}

Test(http_headers, Expect)
{
	cr_assert(strcmp("Expect", HTTP_HEADER_STRINGS[HTTP_EXPECT]) == 0, 
		"Expected 'Expect', not:%s", HTTP_HEADER_STRINGS[HTTP_EXPECT]);
}

Test(http_headers, Expect_CT)
{
	cr_assert(strcmp("Expect-CT", HTTP_HEADER_STRINGS[HTTP_EXPECT_CT]) == 0, 
		"Expected 'Expect-CT', not:%s", HTTP_HEADER_STRINGS[HTTP_EXPECT_CT]);
}

Test(http_headers, expires)
{
	cr_assert(strcmp("Expires", HTTP_HEADER_STRINGS[HTTP_EXPIRES]) == 0, 
		"Expected 'Expires', not:%s", HTTP_HEADER_STRINGS[HTTP_EXPIRES]);
}

Test(http_headers, forwarded)
{
	cr_assert(strcmp("Forwarded", HTTP_HEADER_STRINGS[HTTP_FORWARDED]) == 0, 
		"Expected 'Forwarded', not:%s", HTTP_HEADER_STRINGS[HTTP_FORWARDED]);
}

Test(http_headers, from)
{
	cr_assert(strcmp("From", HTTP_HEADER_STRINGS[HTTP_FROM]) == 0, 
		"Expected 'From', not:%s", HTTP_HEADER_STRINGS[HTTP_FROM]);
}

Test(http_headers, host)
{
	cr_assert(strcmp("Host", HTTP_HEADER_STRINGS[HTTP_HOST]) == 0, 
		"Expected 'Host', not:%s", HTTP_HEADER_STRINGS[HTTP_HOST]);
}

Test(http_headers, if_match)
{
	cr_assert(strcmp("If-Match", HTTP_HEADER_STRINGS[HTTP_IF_MATCH]) == 0, 
		"Expected 'If-Match', not:%s", HTTP_HEADER_STRINGS[HTTP_IF_MATCH]);
}

Test(http_headers, if_modified_since)
{
	cr_assert(strcmp("If-Modified-Since", HTTP_HEADER_STRINGS[HTTP_IF_MODIFIED_SINCE]) == 0, 
		"Expected 'If-Modified-Since', not:%s", HTTP_HEADER_STRINGS[HTTP_IF_MODIFIED_SINCE]);
}

Test(http_headers, if_mone_match)
{
	cr_assert(strcmp("If-None-Match", HTTP_HEADER_STRINGS[HTTP_IF_NONE_MATCH]) == 0, 
		"Expected 'If-None-Match', not:%s", HTTP_HEADER_STRINGS[HTTP_IF_NONE_MATCH]);
}

Test(http_headers, if_range)
{
	cr_assert(strcmp("If-Range", HTTP_HEADER_STRINGS[HTTP_IF_RANGE]) == 0, 
		"Expected 'If-Range', not:%s", HTTP_HEADER_STRINGS[HTTP_IF_RANGE]);
}

Test(http_headers, if_unmodified_since)
{
	cr_assert(strcmp("If-Unmodified-Since", HTTP_HEADER_STRINGS[HTTP_IF_UNMODIFIED_SINCE]) == 0, 
		"Expected 'If-Unmodified-Since', not:%s", HTTP_HEADER_STRINGS[HTTP_IF_UNMODIFIED_SINCE]);
}

Test(http_headers, keep_alive)
{
	cr_assert(strcmp("Keep-Alive", HTTP_HEADER_STRINGS[HTTP_KEEP_ALIVE]) == 0, 
		"Expected 'Keep-Alive', not:%s", HTTP_HEADER_STRINGS[HTTP_KEEP_ALIVE]);
}

Test(http_headers, large_allocation)
{
	cr_assert(strcmp("Large-Allocation", HTTP_HEADER_STRINGS[HTTP_LARGE_ALLOCATION]) == 0, 
		"Expected 'Large-Allocation', not:%s", HTTP_HEADER_STRINGS[HTTP_LARGE_ALLOCATION]);
}

Test(http_headers, last_modified)
{
	cr_assert(strcmp("Last-Modified", HTTP_HEADER_STRINGS[HTTP_LAST_MODIFIED]) == 0, 
		"Expected 'Last-Modified', not:%s", HTTP_HEADER_STRINGS[HTTP_LAST_MODIFIED]);
}

Test(http_headers, location)
{
	cr_assert(strcmp("Location", HTTP_HEADER_STRINGS[HTTP_LOCATION]) == 0, 
		"Expected 'Location', not:%s", HTTP_HEADER_STRINGS[HTTP_LOCATION]);
}

Test(http_headers, origin)
{
	cr_assert(strcmp("Origin", HTTP_HEADER_STRINGS[HTTP_ORIGIN]) == 0, 
		"Expected 'Origin', not:%s", HTTP_HEADER_STRINGS[HTTP_ORIGIN]);
}

Test(http_headers, pragma)
{
	cr_assert(strcmp("Pragma", HTTP_HEADER_STRINGS[HTTP_PRAGMA]) == 0, 
		"Expected 'Pragma', not:%s", HTTP_HEADER_STRINGS[HTTP_PRAGMA]);
}

Test(http_headers, proxy_authenticate)
{
	cr_assert(strcmp("Proxy-Authenticate", HTTP_HEADER_STRINGS[HTTP_PROXY_AUTHENTICATE]) == 0, 
		"Expected 'Proxy-Authenticate', not:%s", HTTP_HEADER_STRINGS[HTTP_PROXY_AUTHENTICATE]);
}

Test(http_headers, proxy_authorization)
{
	cr_assert(strcmp("Proxy-Authorization", HTTP_HEADER_STRINGS[HTTP_PROXY_AUTHORIZATION]) == 0, 
		"Expected 'Proxy-Authorization', not:%s", HTTP_HEADER_STRINGS[HTTP_PROXY_AUTHORIZATION]);
}

Test(http_headers, public_key_pins)
{
	cr_assert(strcmp("Public-Key-Pins", HTTP_HEADER_STRINGS[HTTP_PUBLIC_KEY_PINS]) == 0, 
		"Expected 'Public-Key-Pins', not:%s", HTTP_HEADER_STRINGS[HTTP_PUBLIC_KEY_PINS]);
}

Test(http_headers, public_key_pins_report_only)
{
	cr_assert(strcmp("Public-Key-Pins-Report-Only", HTTP_HEADER_STRINGS[HTTP_PUBLIC_KEY_PINS_REPORT_ONLY]) == 0, 
		"Expected 'Public-Key-Pins-Report-Only', not:%s", HTTP_HEADER_STRINGS[HTTP_PUBLIC_KEY_PINS_REPORT_ONLY]);
}

Test(http_headers, referer)
{
	cr_assert(strcmp("Referer", HTTP_HEADER_STRINGS[HTTP_REFERER]) == 0, 
		"Expected 'Referer', not:%s", HTTP_HEADER_STRINGS[HTTP_REFERER]);
}

Test(http_headers, referrer_policy)
{
	cr_assert(strcmp("Referrer-Policy", HTTP_HEADER_STRINGS[HTTP_REFERRER_POLICY]) == 0, 
		"Expected 'Referrer-Policy', not:%s", HTTP_HEADER_STRINGS[HTTP_REFERRER_POLICY]);
}

Test(http_headers, retry_after)
{
	cr_assert(strcmp("Retry-After", HTTP_HEADER_STRINGS[HTTP_RETRY_AFTER]) == 0, 
		"Expected 'Retry-After', not:%s", HTTP_HEADER_STRINGS[HTTP_RETRY_AFTER]);
}

Test(http_headers, server)
{
	cr_assert(strcmp("Server", HTTP_HEADER_STRINGS[HTTP_SERVER]) == 0, 
		"Expected 'Server', not:%s", HTTP_HEADER_STRINGS[HTTP_SERVER]);
}

Test(http_headers, server_timing)
{
	cr_assert(strcmp("Server-Timing", HTTP_HEADER_STRINGS[HTTP_SERVER_TIMING]) == 0, 
		"Expected 'Server-Timing', not:%s", HTTP_HEADER_STRINGS[HTTP_SERVER_TIMING]);
}

Test(http_headers, set_cookie)
{
	cr_assert(strcmp("Set-Cookie", HTTP_HEADER_STRINGS[HTTP_SET_COOKIE]) == 0, 
		"Expected 'Set-Cookie', not:%s", HTTP_HEADER_STRINGS[HTTP_SET_COOKIE]);
}

Test(http_headers, set_cookie2)
{
	cr_assert(strcmp("Set-Cookie2", HTTP_HEADER_STRINGS[HTTP_SET_COOKIE2]) == 0, 
		"Expected 'Set-Cookie2', not:%s", HTTP_HEADER_STRINGS[HTTP_SET_COOKIE2]);
}

Test(http_headers, sourcemap)
{
	cr_assert(strcmp("SourceMap", HTTP_HEADER_STRINGS[HTTP_SOURCEMAP]) == 0, 
		"Expected 'SourceMap', not:%s", HTTP_HEADER_STRINGS[HTTP_SOURCEMAP]);
}

Test(http_headers, strict_transport_security)
{
	cr_assert(strcmp("Strict-Transport-Security", HTTP_HEADER_STRINGS[HTTP_STRICT_TRANSPORT_SECURITY]) == 0, 
		"Expected 'Strict-Transport-Security', not:%s", HTTP_HEADER_STRINGS[HTTP_STRICT_TRANSPORT_SECURITY]);
}

Test(http_headers, TE)
{
	cr_assert(strcmp("TE", HTTP_HEADER_STRINGS[HTTP_TE]) == 0, 
		"Expected 'TE', not:%s", HTTP_HEADER_STRINGS[HTTP_TE]);
}

Test(http_headers, timing_allow_origin)
{
	cr_assert(strcmp("Timing-Allow-Origin", HTTP_HEADER_STRINGS[HTTP_TIMING_ALLOW_ORIGIN]) == 0, 
		"Expected 'Timing-Allow-Origin', not:%s", HTTP_HEADER_STRINGS[HTTP_TIMING_ALLOW_ORIGIN]);
}

Test(http_headers, Tk)
{
	cr_assert(strcmp("Tk", HTTP_HEADER_STRINGS[HTTP_TK]) == 0, 
		"Expected 'Tk', not:%s", HTTP_HEADER_STRINGS[HTTP_TK]);
}

Test(http_headers, trailer)
{
	cr_assert(strcmp("Trailer", HTTP_HEADER_STRINGS[HTTP_TRAILER]) == 0, 
		"Expected 'Trailer', not:%s", HTTP_HEADER_STRINGS[HTTP_TRAILER]);
}

Test(http_headers, transfer_encoding)
{
	cr_assert(strcmp("Transfer-Encoding", HTTP_HEADER_STRINGS[HTTP_TRANSFER_ENCODING]) == 0, 
		"Expected 'Transfer-Encoding', not:%s", HTTP_HEADER_STRINGS[HTTP_TRANSFER_ENCODING]);
}

Test(http_headers, upgrade_insecure_requests)
{
	cr_assert(strcmp("Upgrade-Insecure-Requests", HTTP_HEADER_STRINGS[HTTP_UPGRADE_INSECURE_REQUESTS]) == 0, 
		"Expected 'Upgrade-Insecure-Requests', not:%s", HTTP_HEADER_STRINGS[HTTP_UPGRADE_INSECURE_REQUESTS]);
}

Test(http_headers, user_agent)
{
	cr_assert(strcmp("User-Agent", HTTP_HEADER_STRINGS[HTTP_USER_AGENT]) == 0, 
		"Expected 'User-Agent', not:%s", HTTP_HEADER_STRINGS[HTTP_USER_AGENT]);
}

Test(http_headers, vary)
{
	cr_assert(strcmp("Vary", HTTP_HEADER_STRINGS[HTTP_VARY]) == 0, 
		"Expected 'Vary', not:%s", HTTP_HEADER_STRINGS[HTTP_VARY]);
}

Test(http_headers, via)
{
	cr_assert(strcmp("Via", HTTP_HEADER_STRINGS[HTTP_VIA]) == 0, 
		"Expected 'Via', not:%s", HTTP_HEADER_STRINGS[HTTP_VIA]);
}

Test(http_headers, WWW_authenticate)
{
	cr_assert(strcmp("WWW-Authenticate", HTTP_HEADER_STRINGS[HTTP_WWW_AUTHENTICATE]) == 0, 
		"Expected 'WWW-Authenticate', not:%s", HTTP_HEADER_STRINGS[HTTP_WWW_AUTHENTICATE]);
}

Test(http_headers, warning)
{
	cr_assert(strcmp("Warning", HTTP_HEADER_STRINGS[HTTP_WARNING]) == 0, 
		"Expected 'Warning', not:%s", HTTP_HEADER_STRINGS[HTTP_WARNING]);
}

Test(http_headers, x_content_type_options)
{
	cr_assert(strcmp("X-Content-Type-Options", HTTP_HEADER_STRINGS[HTTP_X_CONTENT_TYPE_OPTIONS]) == 0, 
		"Expected 'X-Content-Type-Options', not:%s", HTTP_HEADER_STRINGS[HTTP_X_CONTENT_TYPE_OPTIONS]);
}

Test(http_headers, X_DNS_prefetch_control)
{
	cr_assert(strcmp("X-DNS-Prefetch-Control", HTTP_HEADER_STRINGS[HTTP_X_DNS_PREFETCH_CONTROL]) == 0, 
		"Expected 'X-DNS-Prefetch-Control', not:%s", HTTP_HEADER_STRINGS[HTTP_X_DNS_PREFETCH_CONTROL]);
}

Test(http_headers, x_forwarded_for)
{
	cr_assert(strcmp("X-Forwarded-For", HTTP_HEADER_STRINGS[HTTP_X_FORWARDED_FOR]) == 0, 
		"Expected 'X-Forwarded-For', not:%s", HTTP_HEADER_STRINGS[HTTP_X_FORWARDED_FOR]);
}

Test(http_headers, X_forwarded_host)
{
	cr_assert(strcmp("X-Forwarded-Host", HTTP_HEADER_STRINGS[HTTP_X_FORWARDED_HOST]) == 0, 
		"Expected 'X-Forwarded-Host', not:%s", HTTP_HEADER_STRINGS[HTTP_X_FORWARDED_HOST]);
}

Test(http_headers, X_forwarded_proto)
{
	cr_assert(strcmp("X-Forwarded-Proto", HTTP_HEADER_STRINGS[HTTP_X_FORWARDED_PROTO]) == 0, 
		"Expected 'X-Forwarded-Proto', not:%s", HTTP_HEADER_STRINGS[HTTP_X_FORWARDED_PROTO]);
}

Test(http_headers, X_frame_options)
{
	cr_assert(strcmp("X-Frame-Options", HTTP_HEADER_STRINGS[HTTP_X_FRAME_OPTIONS]) == 0, 
		"Expected 'X-Frame-Options', not:%s", HTTP_HEADER_STRINGS[HTTP_X_FRAME_OPTIONS]);
}

Test(http_headers, X_XSS_protection)
{
	cr_assert(strcmp("X-XSS-Protection", HTTP_HEADER_STRINGS[HTTP_X_XSS_PROTECTION]) == 0, 
		"Expected 'X-XSS-Protection', not:%s", HTTP_HEADER_STRINGS[HTTP_X_XSS_PROTECTION]);
}

