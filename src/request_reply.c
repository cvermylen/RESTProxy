#include <stdlib.h>

#include "route_def.h"
#include "shared_buffers.h"
#include "connection.h"
#include "request_reply.h"
#include "socket_connector.h"

void *push_data_2_destination(void *params)
{
        reply_t *reply = (reply_t*)params;
printf("BUFFER:%s\n", reply->request->buffer);
        switch(reply->type){
        case TYPE_SOCKET:
printf("TYPE SOCKET\n");
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

request_t* create_request(const ri_connection_t* conn, int buffer_no, char* buffer)
{
        request_t* request= (request_t*)malloc(sizeof(request_t));
	request->forward_mode = conn->route->forward_mode;
        request->buffer_size = TX_BUFFER_SIZE;
        request->buffer = buffer;
	request->buffer_no = buffer_no;
	request->out_connections = conn->route->out_connections;
	request->in_response.sock_fd = conn->fd;
	request->replies = (reply_t**)malloc(sizeof(reply_t*) * conn->route->out_connections);
	for(int i=0; i < conn->route->out_connections; i++){
		request->replies[i] = create_reply(conn, conn->route->out_connectors[i]);
		request->replies[i]->request = request;
	}
	return request;
}

void reply_to_client(void* thread_data)
{
        reply_t* reply = (reply_t*)thread_data;
        sock_write(reply->request->in_response.sock_fd, reply->response, reply->response_length);
}

void release_request(request_t* d)
{
        //free(d);
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
         free_buffer(request->buffer_no);
         //close(at->route->in_connector->content.sock->fd);
         return NULL;
}

void wait_4_all_sender_to_complete(request_t* request)
{
        int rc = pthread_create(&request->async_replies_thread, NULL, async_join_threads, request);
}

void forward_data(reply_t* reply)
{
	if(reply->request->forward_mode != FORWARD_MODE_SEQ){
		int rc = pthread_create(&reply->pthread, NULL, push_data_2_destination, reply);
	}else{
		push_data_2_destination(reply);
	}
}

void forward_message_to_all_servers(const int data_len, request_t* request)
{
	request->data_len = data_len;
	for(int i=0; i < request->out_connections; i++){
		forward_data(request->replies[i]);
	}
}
