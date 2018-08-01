#include <stdlib.h>
#include <string.h>
#include "file_connector.h"
#include "route_def.h"
#include "request_reply.h"

//char resp[] = "HTTP/1.1 200 OK\nDate: Mon, 23 May 2005 22:38:34 GMT\nContent-Type: text/html; charset=UTF-8\nContent-Length: 138\nLast-Modified: Wed, 08 Jan 2003 23:11:55 GMT\nServer: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\nETag: \"3f80f-1b6-3e1cb03b\"\nAccept-Ranges: bytes\n\n<html>\n<head>\n <title>An Example Page</title>\n</head>\n<body>\n Hello World, this is a very simple HTML document.\n</body>\n</html>";
char resp[] = "HTTP/1.1 200 OK\nDate: Mon, 23 May 2005 22:38:34 GMT\nContent-Type: text/html; charset=UTF-8\nContent-Length: 14\nLast-Modified: Wed, 08 Jan 2003 23:11:55 GMT\nServer: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\nETag: \"3f80f-1b6-3e1cb03b\"\nAccept-Ranges: bytes\n\n<html>\n</html>";

reply_t* create_response(reply_t* data){
	int buff_no = alloc_buffer();
	char* buffer = get_buffer(buff_no);
	int code = http_decode_response_type(buffer, strlen(resp));
	http_message_t* msg = http_message_init(buff_no, buffer, code, strlen(resp));
	data->response_message = msg;
	strcpy(msg->buffer, resp);
	msg->raw_message_length = strlen(resp);
	return data;
}

void file_writer(void *params)
{
	reply_t* reply = (reply_t*) params;
	fputs(reply->request->http_message->buffer, reply->content.file->file);
	fflush(reply->content.file->file);
	if(reply->flow == FLOW_BIDIRECTIONAL){
		reply->response_callback(create_response(reply));
	}
}
