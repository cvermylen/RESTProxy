#include "http_headers.h"
#include "../frame_buffers/shared_buffers.h"
#include "../socket/socket_connector.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

//#include <str_stack.h>

int str2int(char *value, int field_length) {
    int res = 0;
    int exp = field_length - 1;
    for (int i = 0; i < field_length; res += (pow(10, exp--)) * (value[i] - 48), i++);
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

http_header_t* http_headers_init(int fd, char *buffer, int data_len) {
    http_header_t* header = (http_header_t*)malloc(sizeof(http_header_t));
    for (int i = 0; i < NUM_HTTP_HEADERS; i++) {
        header->headers[i] = stack_init();
    }
    header->fd = fd;
    header->buff = buffer;
    header->cur_loc = 0;
    header->last_semicolon = -1;
    header->max_len = data_len;
    header->start_of_line = 0;
    return header;
}

/*
 \pre buff is not NULL
 \pre start_of_line <= cur_loc <= max_len
*/
void skip_eol(http_header_t *header) {
    if (header->cur_loc < header->max_len &&
        (header->buff[header->cur_loc] == 0x0A && header->buff[header->cur_loc + 1] == 0x0D)
        || (header->buff[header->cur_loc] == 0x0D && header->buff[header->cur_loc + 1] == 0x0A)){
        header->cur_loc += 2;
    } else if (header->buff[header->cur_loc] == '\n') {
        header->cur_loc += 1;
    }
    header->start_of_line = header->cur_loc;
}

int is_eol_reached(http_header_t *header) {
    return (header->cur_loc == header->max_len) ||
           (((header->cur_loc < header->max_len) &&
             (header->cur_loc < header->max_len &&
              header->buff[header->cur_loc] == 0x0A &&
              header->buff[header->cur_loc + 1] == 0x0D)) ||
            (header->buff[header->cur_loc] == '\n'));
}

http_header_t *get_next_line(http_header_t *header) {

    printf("get_next_line1, cur_loc:%d, max_len:%d\n", header->cur_loc, header->max_len);
    skip_eol(header);
    printf("get_next_line2, cur_loc:%d, max_len:%d\n", header->cur_loc, header->max_len);
    if (header->cur_loc >= header->max_len) {
        printf("Read from socket\n");
        int sz = read_from_socket(header->fd, &(header->buff[header->max_len]), TX_BUFFER_SIZE);
        header->max_len += (sz > 0) ? (sz) : (0);
        printf("New max_len:%d\n", header->max_len);
    }
    while (!is_eol_reached(header)) {
        if (header->buff[header->cur_loc] == ':')
            header->last_semicolon = header->cur_loc;
        header->cur_loc += 1;
    }
    printf("cur_loc:%d, last_semicolon:%d\n", header->cur_loc, header->last_semicolon);
    return header;
}

int header_strlen(http_header_t *header) {
    return header->cur_loc - header->start_of_line;
}

int is_two_bytes_eol(char *ptr) {
    return ((*ptr == 0x0A && ptr[1] == 0x0D) || (*ptr == 0x0D && ptr[1] == 0x0A));
}

int is_one_byte_eol(char *ptr) {
    return (*ptr == '\n');
}

int is_eol(char *ptr) {
    if (*ptr != 0x00 && ptr[1] != 0x00 && ptr[2] != 0x00) {
        if (is_two_bytes_eol(ptr)) return 2;
        else if (is_one_byte_eol(ptr)) return 1;
        else return 0;
    } else return 0;
}

//TODO generalize the use of these methods & avoid spreading 0x0A all way around
int is_eof(char *ptr) {
    return (*ptr == 0x00);
}

char *strmncpy(char *buffer, int start, int end) {
    //TODO We could use static buffers defined at the thread level
    char *result = (char *) malloc(sizeof(char) * (end - start + 1));
    strncpy(result, &buffer[start], end - start);
    result[end - start] = '\0';
    return result;
}

char *http_headers_get_header_value(http_header_t *header) {
    int start_value_pos = header->last_semicolon + 1;
    while (header->buff[start_value_pos] == 0x20) start_value_pos++;
    int offset = is_eof(&header->buff[header->cur_loc]) ? is_eol(&header->buff[header->cur_loc]) : 0;
    printf("Offset:%d\n", offset);
    char *value = strmncpy(header->buff, start_value_pos, header->cur_loc - offset);
    printf("Length:%d\n", header->cur_loc - offset);
    printf("value:[%s]\n", value);
    return value;
}

void http_headers_add(http_header_t *header) {
    char *key = strmncpy(header->buff, header->start_of_line, header->last_semicolon);
    int index = find_header_index(key);
    char *value = NULL;
    if (index >= 0) {
        value = http_headers_get_header_value(header);
        str_stack_push(header->headers[index], value);
        printf("pushing:%d---%s\n", index, value);
        printf("========\n");
        printf("------------------\n%s===============\n", http_headers_to_string(header));
        free(value);
        printf("------------------\n%s===============\n", http_headers_to_string(header));
    }
    free(key);

}


stack_head_t *http_headers_get(http_header_t *header, const int prop_key) {
    stack_head_t *result = NULL;
    if (prop_key >= 0 && prop_key < NUM_HTTP_HEADERS) {
        result = header->headers[prop_key];
    }
    return result;
}

int decode_http_headers(http_header_t *header) {
    printf("decode_http_headers\n");
    int cont = 1;
    get_next_line(header);
    while (cont && header_strlen(get_next_line(header)) > 0) {
        if (header->last_semicolon > header->start_of_line) {
            http_headers_add(header);
        } else {
            cont = 0;
        }
    }
    return header->cur_loc;
}

void http_headers_free(http_header_t *header) {
    for (int i = 0; i < NUM_HTTP_HEADERS; i++) {
		str_stack_free(header->headers[i]);
    }
}

int find_header_index(const char *header) {
    size_t i = NUM_HTTP_HEADERS / 2;
    int length = NUM_HTTP_HEADERS;
    while (i < length) {
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

char* http_headers_to_string(http_header_t* header)
{
    char* result = (char*) malloc(sizeof(char) * 1);
    result[0] = '\0';
    for (int i = 0; i < NUM_HTTP_HEADERS; i++) {
        if (header->headers[i]->num_elems > 0) {
            char* tmp = result;
            char* line = str_stack_2_formatted_line(header->headers[i]);
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
