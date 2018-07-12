#ifndef REQUEST_REPLY_H
#define REQUEST_REPLY_H

#include "hashmap.h"
#include "connection.h"
#include "route_instance.h"

typedef struct request request_t;

typedef struct {
	request_t* request;
        pthread_t pthread;
        int flow;
        int type;
        union {
                ri_sock_connector_t *sock;
                ri_file_connector_t *file;
                ri_workflow_connector_t *workflow;
        } content;
        char* response;
        int response_length;
        void (*response_callback)(void* ri_response);
} reply_t;

struct request{
	int forward_mode;
	int buffer_no;
        int buffer_size;
        char *buffer;
        int data_len;
	hashmap_t* http_headers;
        union {
                int sock_fd;
                FILE* file_fd;
        }in_response;
	int out_connections;
	reply_t** replies;
	pthread_t async_replies_thread;
};

request_t* create_request(const ri_connection_t* conn, int buffer_no, char* buffer);

/*!
  \brief Data that an instance of an out connector needs to send out. The connector instance may be hold in a separate thread.
 */
request_t* create_runtime_thread_data(int fd, ri_in_connector_t* in, ri_out_connector_t *conn, int buffer_size, char *buffer, int data_len);

void release_conn_runtime_thread_data(request_t* d);

void reply_to_client(void* thread_data);

void forward_message_to_all_servers(const int data_len, request_t* request);

void* push_data_2_destination(void* params);
void wait_4_all_sender_to_complete(request_t* request);
void release_request(request_t* request);
#endif
