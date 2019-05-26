#include "http_headers.h"
#include "../buffers/shared_buffers.h"
#include "../socket/socket_connector.h"
#include "../buffers/circular_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

int str2int(const char *value, int field_length) {
    int res = 0;
    int exp = field_length - 1;
    for (int i = 0; i < field_length; res += ((int)pow(10, exp--)) * (value[i] - 48), i++);
    return res;
}

int decode_body_length(http_header_t *header) {
    int result = 0;
	stack_head_t* st = http_headers_get(header, HTTP_CONTENT_LENGTH);
	char* value = str_stack_top(st);
	if(value != NULL){
		result = str2int(value, strlen(value));
	}
    return result;
}

//TODO REFACTOR. headers needs to get its data from the message, and a message contains the headers.
// BUT we want to avoid a circular dependency. Callbacks is not the nicest way, as there is no reason
// for genericity.
//
http_header_t* http_headers_init(circular_buffer_t* buffers) {
    http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
    for (int i = 0; i < NUM_HTTP_HEADERS; i++) {
        header->headers[i] = stack_init();
    }
    header->buffers = buffers;

    header->cur_loc.circ_index = 0;
    header->cur_loc.buff_pos = 0;
    header->last_semicolon.buff_pos = -1;
    header->last_semicolon.circ_index = -1;
    header->max_len.buff_pos = 0;
    header->max_len.circ_index = 0;
    header->start_of_line.circ_index = 0;
    header->start_of_line.buff_pos = 0;
    return header;
}


int is_two_bytes_eol (char *ptr) {
    return ((ptr[0] == 0x0A && ptr[1] == 0x0D) || (ptr[0] == 0x0D && ptr[1] == 0x0A));
}

int is_one_byte_eol(char ptr) {
    return (ptr == '\n');
}

int is_eol (char *ptr, int size)
{
    printf("Size:%d\n", size);
    printf("First:%x second:%x\n", ptr[0], ptr[1]);
    int result = 0;
    if (size > 0) {
        if (size >= 2 && is_two_bytes_eol(ptr)) {
            result = 2;
        }
        if (!result && is_one_byte_eol(ptr[0])) {
            result = 1;
        }
        printf("Intermediary result:%d\n", result);
    } else {
        result = -1;
    }
    return result;
}

int is_ptr_pointing_to_eol_or_eos (circular_buffer_t* cb, circular_ptr_t* ptr, circular_ptr_t* max_len)
{
    char* ptr_to_data = get_char_ptr_from_buffer(cb, ptr);
    int distance_to_end_of_buffer =  cb->data_sizes[ptr->circ_index] - ptr->buff_pos;
    printf("Distance:%d\n", distance_to_end_of_buffer);
    int result;
    int eof = 0;
    if (distance_to_end_of_buffer < 2) {
        int bytes_read = cb->data_sizes[circular_increment(ptr->circ_index, cb->size)];
        if (bytes_read < 0)
            bytes_read = feed_next_buffer(cb, max_len);
        printf("Size of data read:%d\n", bytes_read);
        if (bytes_read > 0) {
            circular_ptr_t *next = (circular_ptr_t *) malloc(sizeof(circular_ptr_t));
            copy_circ_pointers(next, ptr);
            op_add_circ_pointers(cb, next, 1);
            if (distance_to_end_of_buffer == 1) {
                char first = ptr_to_data[0];
                char* tmp_str = (char *) malloc(sizeof(char) * 3);
                tmp_str[0] = first;
                char *second_part = get_char_ptr_from_buffer(cb, next);
                tmp_str[1] = second_part[0];
                tmp_str[2] = '\0';
                distance_to_end_of_buffer = 2;
                free(tmp_str);
            } else {
                ptr_to_data = get_char_ptr_from_buffer(cb, next);
                distance_to_end_of_buffer = cb->data_sizes[next->circ_index] - next->buff_pos;
            }
            free(next);
        } else {
            eof = 1;
        }
    }
    result = is_eol(ptr_to_data, distance_to_end_of_buffer);
    result |= ( eof && (distance_to_end_of_buffer == 0));
    return result;
}
/*
 \pre buff is not NULL
 \pre start_of_line <= cur_loc <= max_len
*/
void skip_eol_if_present (http_header_t *header) {

    int offset = is_eol(get_char_ptr_from_buffer(header->buffers, &(header->cur_loc)), header->buffers->data_sizes[header->cur_loc.circ_index] - header->cur_loc.buff_pos);
    if (offset > 0) {
        op_add_circ_pointers(header->buffers, &(header->cur_loc), offset);
        copy_circ_pointers(&(header->start_of_line), &(header->cur_loc));
    }
}

http_header_t* get_next_line (http_header_t* header)
{

    printf("get_next_line1, cur_loc:[%d-%d], max_len:[%d-%d]\n", header->cur_loc.circ_index, header->cur_loc.buff_pos, header->max_len.circ_index, header->max_len.buff_pos);
    if (is_empty_circular_buffer(header->buffers) || (cmp_ptr_to_last_received(header->buffers, &(header->cur_loc)) >= 0)) {
        printf("Read from socket\n");
        feed_next_buffer(header->buffers, &(header->max_len));
//        int sz = read_from_socket(header->fd, &(header->buff[header->max_len]), TX_BUFFER_SIZE);
        printf("New max_len:[%d-%d]\n", header->max_len.circ_index, header->max_len.buff_pos);
    }
    skip_eol_if_present(header);
    printf("get_next_line2, cur_loc:[%d-%d], max_len:[%d-%d]\n", header->cur_loc.circ_index, header->cur_loc.buff_pos, header->max_len.circ_index, header->max_len.buff_pos);
    while (!is_ptr_pointing_to_eol_or_eos (header->buffers, &(header->cur_loc), &(header->max_len))) {
        if (get_char_ptr_from_buffer(header->buffers, &(header->cur_loc))[0] == ':')
            copy_circ_pointers (&(header->last_semicolon), &(header->cur_loc));
        op_add_circ_pointers(header->buffers, &(header->cur_loc), 1);
        printf("curr_loc.circ_index:%d curr_loc.buff_pos:%d\n", header->cur_loc.circ_index, header->cur_loc.buff_pos);
    }
    printf("cur_loc:[%d-%d], last_semicolon:[%d-%d]\n", header->cur_loc.circ_index, header->cur_loc.buff_pos,
            header->last_semicolon.circ_index, header->last_semicolon.buff_pos);
    return header;
}

int header_strlen (http_header_t *header) {
    return op_distance_circ_pointers(header->buffers, &(header->start_of_line), &(header->cur_loc));
}


int is_eof(char *ptr) {
    return (ptr[0] == 0x00);
}

char *strmncpy(char *buffer, int start, int end) {
    //TODO We could use static buffers defined at the thread level
    char *result = (char *) malloc(sizeof(char) * (end - start + 1));
    strncpy(result, &buffer[start], end - start);
    result[end - start] = '\0';
    return result;
}


char *http_headers_get_header_value(http_header_t *header) {
    do {
        op_add_circ_pointers(header->buffers, &(header->last_semicolon), 1);
    } while(get_char_ptr_from_buffer(header->buffers, &(header->last_semicolon))[0] == 0x20);
    int offset = is_eof(get_char_ptr_from_buffer(header->buffers, &(header->cur_loc))) ?
            is_eol(get_char_ptr_from_buffer(header->buffers, &(header->cur_loc)), header->buffers->data_sizes[header->cur_loc.circ_index] - header->cur_loc.buff_pos) : 0;
    printf("Offset:%d\n", offset);
    circular_ptr_t* end_ptr = op_sub_circ_pointer(header->buffers, &(header->cur_loc), offset);
    char* value = buffer_2_str_copy(header->buffers, &(header->last_semicolon), end_ptr);
    free(end_ptr);
    printf("value:[%s]\n", value);
    return value;
}

void http_headers_add(http_header_t* hdr)
{
    char *key = buffer_2_str_copy(hdr->buffers, &(hdr->start_of_line), &(hdr->last_semicolon));
    int index = find_header_index(key);
    char *value = NULL;
    if (index >= 0) {
        value = http_headers_get_header_value(hdr);
        printf("Will push value:%s\n", value);
        str_stack_push(hdr->headers[index], value);
        printf("pushing:%d---%s\n", index, value);
        printf("========\n");
        printf("------------------\n%s===============\n", http_headers_to_string(hdr));
        free(value);
        printf("------------------\n%s===============\n", http_headers_to_string(hdr));
    }
    free(key);

}

stack_head_t *http_headers_get(http_header_t* hdr, const int prop_key)
{
    stack_head_t *result = NULL;
    if (prop_key >= 0 && prop_key < NUM_HTTP_HEADERS) {
        result = hdr->headers[prop_key];
    }
    return result;
}

void decode_http_headers(http_header_t* hdr)
{
    printf("decode_http_headers\n");
    int cont = 1;
    get_next_line (hdr);
    while (cont && header_strlen(get_next_line(hdr)) > 0) {
        if (cmp_circular_ptr(hdr->buffers, &(hdr->last_semicolon), &(hdr->start_of_line))) { //last_semicolon > start_of_line
            http_headers_add(hdr);
        } else {
            cont = 0;
        }
    }
}

void http_headers_free(http_header_t* hdr)
{
    for (int i = 0; i < NUM_HTTP_HEADERS; i++) {
		str_stack_free(hdr->headers[i]);
    }
}

int find_header_index(const char* key) {
    size_t i = NUM_HTTP_HEADERS / 2;
    int length = NUM_HTTP_HEADERS;
    while (i < length) {
        int comparison = strcmp(HTTP_HEADER_STRINGS[i], key);
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

char* http_headers_to_string(http_header_t* hdr)
{
    char* result = (char*) malloc(sizeof(char) * 1);
    result[0] = '\0';
    for (int i = 0; i < NUM_HTTP_HEADERS; i++) {
        if (hdr->headers[i]->num_elems > 0) {
            char* tmp = result;
            char* line = str_stack_2_formatted_line(hdr->headers[i]);
            int extension = strlen(line) + 2 + strlen(HTTP_HEADER_STRINGS[i]) + 3;
            result = (char*) malloc ((sizeof(char) * (extension + strlen(tmp))));
            strcpy(result, tmp);
            strcpy(&result[strlen(tmp)], HTTP_HEADER_STRINGS[i]);
            strcpy(&result[strlen(tmp) + strlen(HTTP_HEADER_STRINGS[i])], " : ");
            strcpy(&result[strlen(tmp) + strlen(HTTP_HEADER_STRINGS[i]) + 3], line);
            result[extension + strlen(tmp) - 2] = '\n';
            result[extension + strlen(tmp) - 1] = '\0';
        }
    }
    return result;
}

char *HTTP_HEADER_STRINGS[] = {
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
