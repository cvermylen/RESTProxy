#include "socket_connector.h"
#include "accept_connections.h"
#include "socket_io.h"
#include "shared_buffers.h"
#include "routing_handler.h"
#include "route_instance.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

struct per_conn_recv_result {
	int total_messages;
	long total_bytes;
	int last_result;
	int error;
};

#define NUM_THREADS	1

void *push_data_2_destination(void *params)
{
	ri_thread_data_t *dest = (ri_thread_data_t*)params;
printf("BUFFER:%s\n", dest->buffer);
	switch(dest->type){
	case TYPE_SOCKET:
printf("TYPE SOCKET\n");
		break;
	case TYPE_FILE:
printf("callback is:%s\n", (dest->content.file->file == NULL)?("NULL"):("NOT NULL"));
		dest->content.file->output_callback(dest);
		break;
	case TYPE_WORKFLOW:
printf("TYPE WORKFLOW\n");
		break;
	default:
		printf("Argl\n");
	}
}

struct async_threads_t {
	int *dest_threads;
	int buffer_no;
	ri_route_t *route;
};

struct output_data_t {
	char *buffer;
	int data_size;
	int forward_mode;
	rt_out_connector_t **out_connectors;
};

void* async_join_threads(void * params)
{
	struct async_threads_t *at = (struct async_threads_t*) params;
printf("Joining results...\n");
	int i = 0;
	for(int i=0; i < at->route->out_connections; i++){
printf("Join thread at:%d\n", at->dest_threads[i]);
printf("Is data thread NULL:%s\n", (at->route->out_connectors[i]->data[at->dest_threads[i]]==NULL)?("YES"):("NO"));
		pthread_join(at->route->out_connectors[i]->data[at->dest_threads[i]]->pthread, NULL);
printf("Just joined\n");
		release_conn_runtime_thread_data(at->route->out_connectors[i]->data[at->dest_threads[i]]);
		at->route->out_connectors[i]->data[at->dest_threads[i]] = NULL;
	}
printf("...joined\n");
	free_buffer(at->buffer_no);
	return NULL;
}

void wait_4_all_sender_to_complete(ri_route_t *route, int buffer_no, int *dest_threads)
{
	pthread_t sync_thread;
	struct async_threads_t *at = (struct async_threads_t*)malloc(sizeof(struct async_threads_t));
	at->dest_threads = dest_threads;
	at->route = route;
	at->buffer_no = buffer_no;
	int rc = pthread_create(&sync_thread, NULL, async_join_threads, at);
}

int forward_data(int thread_data_index, int data_size, int forward_mode, ri_out_connector_t *conn)
{
	if(forward_mode !=FORWARD_MODE_SEQ){
printf("Threaded call index:%d\n", thread_data_index);
		int rc = pthread_create(&conn->data[thread_data_index]->pthread, NULL, push_data_2_destination, conn->data[thread_data_index]);
printf("Thread return code %d\n", rc);
	}else{
		push_data_2_destination(conn->data[thread_data_index]);
	}
	return 0;
}

void release_buffer_after_processing(int forward_mode, int buffer_no, ri_route_t *route, int *thread_indexes)
{
printf("Release_buffer_after_processing\n");
	int i;
	switch(route->forward_mode){
	case FORWARD_MODE_SYNC:
		for(i=0; i < route->out_connections; i++){
			pthread_join(route->out_connectors[i]->data[thread_indexes[i]]->pthread, NULL);
		}
		//No break here
	case FORWARD_MODE_SEQ:
		free_buffer(buffer_no);
		break;
	case FORWARD_MODE_ASYNC:
		wait_4_all_sender_to_complete(route, buffer_no, thread_indexes);
		break;
	}
printf("release_buffer_after_processing:END\n");
}

void forward_data_to_all_subscribers(int data_size, ri_route_t *route, int *thread_data_indexes)
{
printf("forward to ALL\n");
	int i;
	for(i=0; i < route->out_connections; i++){
		forward_data(thread_data_indexes[i], data_size, route->forward_mode, route->out_connectors[i]);
	}
}

void *receive_data(void *params)
{
	ri_route_t* route = (ri_route_t*)params;
	struct per_conn_recv_result *res = (struct per_conn_recv_result*)malloc(sizeof(struct per_conn_recv_result));
	res->total_messages = 0;
	res->total_bytes = 0;
	int data_len = 0;
	do {
		int buff = alloc_buffer();
printf("Get new buffer %d\n", buff);
		char *buffer = get_buffer(buff);
		data_len = sock_read(route->in_connector->content.sock->fd, buffer, TX_BUFFER_SIZE, 0);
		int* thread_pool = create_all_runtime_thread_data(route, TX_BUFFER_SIZE, buffer, data_len);
		if(data_len > 0) {
printf("Received:%d\n", data_len);
			res->total_messages += 1;
			res->total_bytes += res->last_result;
			forward_data_to_all_subscribers(res->last_result, route, thread_pool);
			release_buffer_after_processing(route->forward_mode, buff, route, thread_pool);
		}else{
printf("Error code received, free:%d\n", buff);
			res->error = errno;
			free_buffer(buff);
		}
	} while(data_len > 0);
	close_connection(route);
	pthread_exit(res);
}

void* socket_connector(void* param)
{
	rt_route_t* route = (rt_route_t*)param;
printf("socket_connector\n");
	pthread_t thread;
	//int newfd = bind_port(port);
	do {
		ri_route_t* cli = wait_4_connection_request(route);
		if(cli->in_connector->content.sock->fd > 0){
			printf("%d\n", cli->in_connector->content.sock->fd);
			int rc = pthread_create(&thread, NULL, receive_data, (void *)cli);
			struct per_conn_recv_result *res;
			//pthread_join(thread, (void*)&res);
			printf("Code %s\n", (res == NULL)?("NULL"):("NOT NULL"));
			//printf("Bytes:%ld\n", res->total_bytes);
		}
	} while(1);
}
