#include <stdlib.h>

#include "route_def.h"
#include "shared_buffers.h"
#include "connection.h"
#include "request_reply.h"
#include "socket_connector.h"
#include "http_message.h"
#include "http_first_line.h"

void *push_data_2_destination(void *params)
{
        reply_t *reply = (reply_t*)params;
printf("BUFFER:%s\n", reply->request->http_message->buffer);
        switch(reply->type){
        case TYPE_SOCKET:
		reply->content.sock->consumer_callback(reply);
                break;
        case TYPE_FILE:
printf("callback is:%s\n", (reply->content.file->file == NULL)?("NULL"):("NOT NULL"));
                reply->content.file->output_callback(reply);
                break;
        default:
                printf("Argl\n");
        }
        return NULL;
}

reply_t* create_reply(const ri_connection_t* conn, ri_out_connector_t* out_conn)
{
	reply_t* reply = (reply_t*)malloc(sizeof(reply_t));
	reply->flow = out_conn->flow;
	reply->type = out_conn->type;
	switch(reply->type){
	case TYPE_SOCKET:
		reply->content.sock = out_conn->content.sock;
		break;
	case TYPE_FILE:
		reply->content.file = out_conn->content.file;
		break;
	}
	reply->response_callback = out_conn->response_callback;
	return reply;
}

request_t* create_request(const ri_connection_t* conn, int buff_no, char* buffer, int code, int sz)
{
        request_t* request= (request_t*)malloc(sizeof(request_t));
	request->forward_mode = conn->route->forward_mode;
        request->buffer_size = TX_BUFFER_SIZE;
	request->http_message = http_message_init(buff_no, buffer, code, sz);
	request->out_connections = conn->route->out_connections;
	request->in_response.sock_fd = conn->fd;
	request->replies = (reply_t**)malloc(sizeof(reply_t*) * conn->route->out_connections);
	for(int i=0; i < conn->route->out_connections; i++){
		request->replies[i] = create_reply(conn, conn->route->out_connectors[i]);
		request->replies[i]->request = request;
	}
	return request;
}

/*void read_full_http_GET_request(int fd, http_message_t* msg)
{
printf("read_full_http_GET_request\n");
        int n, pos = 0;
        do {
                n = read_from_buffer(fd, msg, pos);
                http_message_update_status(msg, pos+1, n);
		pos += n + 1;
        } while(msg->status != HTTP_MSG_STATUS_HEADER_COMPLETE);
}*/

request_t* accept_opening_request_from_client(const ri_connection_t* conn)
{
printf("accept_opening_request_from_client\n");
	int buff_no = alloc_buffer();
	char* buffer = get_buffer(buff_no);
	int sz = 0;
	sz = read_from_socket(conn->fd, buffer, TX_BUFFER_SIZE);
	if(sz ==0) return NULL;
	int code = http_decode_request_type(buffer, sz);
	request_t* request = NULL;
	if(code > 0){
		request = create_request(conn, buff_no, buffer, code, sz);
		switch(code){
		case HTTP_REQUEST_GET:
			//read_full_http_GET_request(conn->fd, request->http_message);
			break;
		case HTTP_REQUEST_POST:
			printf("Not implemented yet\n");
			exit(0);
		}
printf("%s\n", buffer);
	}
        return request;
}

void reply_to_client(void* thread_data)
{
printf("reply_to_client\n");
        reply_t* reply = (reply_t*)thread_data;
        sock_write(reply->request->in_response.sock_fd, reply->response_message->buffer, reply->response_message->raw_message_length);
printf("sent to client\n");
}

void release_reply(reply_t* reply)
{
	http_message_free(reply->response_message);
	free(reply);
}

void release_request(request_t* d)
{
	http_message_free(d->http_message);
	for(int i = 0; i < d->out_connections; i++){
		release_reply(d->replies[i]);
	}
        free(d);
}

void* async_join_threads(void* params)
{
	request_t* request = (request_t*)params;
 printf("Joining results...\n");
        int i = 0;   
        for(int i=0; i < request->out_connections; i++){
 printf("Join thread at:%d\n", request->replies[i]->pthread);
                 pthread_join(request->replies[i]->pthread, NULL);
                 //release_conn_runtime_thread_data(at->route->out_connectors[i]->data[at->dest_threads[i]    ]);
        }
 printf("...joined\n");
	release_request(request);
        free_buffer(request->http_message->buffer_no);
        //close(at->route->in_connector->content.sock->fd);
        return NULL;
}

void wait_4_all_sender_to_complete(request_t* request)
{
        int rc = pthread_create(&request->async_replies_thread, NULL, async_join_threads, request);
}

void forward_to_one_server(reply_t* reply)
{
	if(reply->request->forward_mode != FORWARD_MODE_SEQ){
		int rc = pthread_create(&reply->pthread, NULL, push_data_2_destination, reply);
	}else{
		push_data_2_destination(reply);
	}
}

void forward_message_to_all_servers(request_t* request)
{
	for(int i=0; i < request->out_connections; i++){
		forward_to_one_server(request->replies[i]);
	}
}

void decode_request_message_header(request_t* request)
{
	decode_http_message_header(request->in_response.sock_fd, request->http_message);
}

void process_request_message_body(request_t* request)
{
	switch(request->http_message->function){
	case HTTP_REQUEST_GET:
		break;
	case HTTP_REQUEST_POST:
		break;
	}
        forward_message_to_all_servers(request);
}

void* sync_request_reply_to_server(reply_t* reply)
{
        int socket = connect_to_server(reply->content.sock->server_name, reply->content.sock->port);
        reply->content.sock->fd = socket;
        sock_write(socket, reply->request->http_message->buffer, reply->request->http_message->raw_message_length);
printf("$$$Message sent:\n");
	receive_reply(reply);
        reply->response_callback(reply);
}
