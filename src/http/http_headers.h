#ifndef HTTP_HEADERS_H
#define HTTP_HEADERS_H

#include <str_stack.h>
#include "../buffers/circular_buffer.h"

#define NUM_HTTP_HEADERS			81

typedef struct {
	stack_head_t* headers[NUM_HTTP_HEADERS];

    circular_buffer_t* buffers;

	char* buff; //TODO should be eliminated and replaced by a pointer to the message
    circular_ptr_t start_of_line;
    circular_ptr_t last_semicolon; // Initial value of [-1, -1], re-initialized after a new header has been processed
    circular_ptr_t cur_loc;
    circular_ptr_t max_len;  // Initial value of [-1, -1]

    unsigned int header_size;
} http_header_t;

/*!
 * \brief Allocate and initialize the header with as many entries as defined per 'NUM_HTTP_HEADERS'.
 * DOES NOT initialize the values to start the parsing though. (maybe change that ...)
 * @return
 */
http_header_t* http_headers_init(circular_buffer_t* buffers);

char* strmncpy(char* buffer, int start, int end);

/*!
 * \private
 * \brief The http header 'Body Length' contains a decimal value that is used to parse the actual message. In the header,
 * the value is stored as a String
 * @param header
 * @return
 */
int decode_body_length(http_header_t* header);

/*!
 * Reads the next header line from the I/O buffer. If needed, the buffer can be replenish with a call to the I/O
 * subsystem, as many times as needed until an 'end of line' is found. 'End of line' (EOL) can either be a single
 * '\n' (unix-style) or a 2-bytes '0x0A 0x0D' MS-style. Note we will handle these 2 bytes in any order, as some
 * hand-made test cases are nuts.
 * @param header
 * @return
 */
http_header_t* get_next_line(http_header_t* header);

void http_headers_free(http_header_t* hdr);

stack_head_t* http_headers_get(http_header_t* hdr, const int prop_key);

void http_headers_add(http_header_t* hdr);

void decode_http_headers(http_header_t* hdr);

int find_header_index(const char* key);

int header_strlen(http_header_t* header);

int str2int(const char* value, int field_length);

char* http_headers_to_string(http_header_t* hdr);

extern char* HTTP_HEADER_STRINGS[];

#define HTTP_ACCEPT					0
#define HTTP_ACCEPT_CHARSET				1
#define HTTP_ACCEPT_ENCODING				2
#define HTTP_ACCEPT_LANGUAGE				3
#define HTTP_ACCEPT_RANGES				4
#define HTTP_ACCESS_CONTROL_ALLOW_CREDENTIALS	5
#define HTTP_ACCESS_CONTROL_ALLOW_HEADERS		6
#define HTTP_ACCESS_CONTROL_ALLOW_METHODS		7
#define HTTP_ACCESS_CONTROL_ALLOW_ORIGIN		8
#define HTTP_ACCESS_CONTROL_EXPOSE_HEADERS		9
#define HTTP_ACCESS_CONTROL_MAX_AGE			10
#define HTTP_ACCESS_CONTROL_REQUEST_HEADERS		11
#define HTTP_ACCESS_CONTROL_REQUEST_METHOD		12
#define HTTP_AGE					13
#define HTTP_ALLOW					14
#define HTTP_AUTHORIZATION				15
#define HTTP_CACHE_CONTROL				16
#define HTTP_CONNECTION				17
#define HTTP_CONTENT_DISPOSITION			18
#define HTTP_CONTENT_ENCODING			19
#define HTTP_CONTENT_LANGUAGE			20
#define HTTP_CONTENT_LENGTH				21
#define HTTP_CONTENT_LOCATION			22
#define HTTP_CONTENT_RANGE				23
#define HTTP_CONTENT_SECURITY_POLICY			24
#define HTTP_CONTENT_SECURITY_POLICY_REPORT_ONLY	25
#define HTTP_CONTENT_TYPE				26
#define HTTP_COOKIE					27
#define HTTP_COOKIE2					28
#define HTTP_DNT					29
#define HTTP_DATE					30
#define HTTP_ETAG					31
#define HTTP_EXPECT					32
#define HTTP_EXPECT_CT				33
#define HTTP_EXPIRES					34
#define HTTP_FORWARDED				35
#define HTTP_FROM					36
#define HTTP_HOST					37
#define HTTP_IF_MATCH				38
#define HTTP_IF_MODIFIED_SINCE			39
#define HTTP_IF_NONE_MATCH				40
#define HTTP_IF_RANGE				41
#define HTTP_IF_UNMODIFIED_SINCE			42
#define HTTP_KEEP_ALIVE				43
#define HTTP_LARGE_ALLOCATION			44
#define HTTP_LAST_MODIFIED				45
#define HTTP_LOCATION				46
#define HTTP_ORIGIN					47
#define HTTP_PRAGMA					48
#define HTTP_PROXY_AUTHENTICATE			49
#define HTTP_PROXY_AUTHORIZATION			50
#define HTTP_PUBLIC_KEY_PINS				51
#define HTTP_PUBLIC_KEY_PINS_REPORT_ONLY		52
#define HTTP_RANGE					53
#define HTTP_REFERER					54
#define HTTP_REFERRER_POLICY				55
#define HTTP_RETRY_AFTER				56
#define HTTP_SERVER					57
#define HTTP_SERVER_TIMING				58
#define HTTP_SET_COOKIE				59
#define HTTP_SET_COOKIE2				60
#define HTTP_SOURCEMAP				61
#define HTTP_STRICT_TRANSPORT_SECURITY		62
#define HTTP_TE					63
#define HTTP_TIMING_ALLOW_ORIGIN			64
#define HTTP_TK					65
#define HTTP_TRAILER					66
#define HTTP_TRANSFER_ENCODING			67
#define HTTP_UPGRADE_INSECURE_REQUESTS		68
#define HTTP_USER_AGENT				69
#define HTTP_VARY					70
#define HTTP_VIA					71
#define HTTP_WWW_AUTHENTICATE			72
#define HTTP_WARNING					73
#define HTTP_X_CONTENT_TYPE_OPTIONS			74
#define HTTP_X_DNS_PREFETCH_CONTROL			75
#define HTTP_X_FORWARDED_FOR				76
#define HTTP_X_FORWARDED_HOST			77
#define HTTP_X_FORWARDED_PROTO			78
#define HTTP_X_FRAME_OPTIONS				79
#define HTTP_X_XSS_PROTECTION			80

#endif
