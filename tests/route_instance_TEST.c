#include <criterion/criterion.h>
#include <string.h>

#include "../src/route_instance.h"
#include "../src/route_def.h"

static ri_sock_connector_t* mock_sock_connector()
{
	ri_sock_connector_t* res = (ri_sock_connector_t*)malloc(sizeof(ri_sock_connector_t));
	res->port = 1;
	res->mode = 2;
	return res;
}

static ri_sock_connector_t* mock_ri_sock_connector()
{
	ri_sock_connector_t* res = (ri_sock_connector_t*)malloc(sizeof(ri_sock_connector_t));
	res->port = 1;
	res->mode = 2;
	res->fd = 3;
	return res;
}

static ri_file_connector_t* mock_file_connector()
{
	ri_file_connector_t* res = (ri_file_connector_t*)malloc(sizeof(ri_file_connector_t));
	res->filename = (char*)malloc(sizeof(char));
	res->filename[0] = '\0';
	return res;
}

static ri_file_connector_t* mock_ri_file_connector()
{
	ri_file_connector_t* res = (ri_file_connector_t*)malloc(sizeof(ri_file_connector_t));
	res->filename = (char*)malloc(sizeof(char));
	res->filename[0] = '\0';
	res->file = (FILE*)malloc(sizeof(FILE));
	return res;
}

static ri_out_connector_t* mock_ri_out_connector(const int type)
{
	ri_out_connector_t* res = (ri_out_connector_t*)malloc(sizeof(ri_out_connector_t));
	res->type = type;
	switch(type){
	case TYPE_SOCKET:
		res->content.sock = mock_ri_sock_connector();
		break;
	case TYPE_FILE:
		res->content.file = mock_ri_file_connector();
		break;
	case TYPE_WORKFLOW:
		res->content.workflow = NULL;
		break;
	}
	return res;
}

static ri_out_connector_t* mock_out_connector(const int type)
{
	ri_out_connector_t* res = (ri_out_connector_t*)malloc(sizeof(ri_out_connector_t));
	res->type = type;
	switch(type){
	case TYPE_SOCKET:
		//res->content.sock = mock_sock_connector();
		break;
	case TYPE_FILE:
		//res->content.file = mock_file_connector();
		break;
	}
	return res;
}

static ri_in_connector_t* mock_ri_in_connector(const int type)
{
	ri_in_connector_t* res = (ri_in_connector_t*)malloc(sizeof(ri_in_connector_t));
	res->type = type;
	switch(type){
	case TYPE_SOCKET:
		res->content.sock = mock_ri_sock_connector();
		break;
	case TYPE_FILE:
		res->content.file = mock_ri_file_connector();
		break;
	}
	return res;
}

static ri_route_t* mock_route(const int out_conn)
{
	ri_route_t* route = (ri_route_t*)malloc(sizeof(ri_route_t));
	route->in_connector = mock_ri_in_connector(TYPE_SOCKET);
	route->out_connections = out_conn;
	route->out_connectors = (ri_out_connector_t**)malloc(sizeof(ri_out_connector_t*) * out_conn);
	for(int i = 0; i < out_conn; i++){
		route->out_connectors[i] = mock_ri_out_connector(TYPE_FILE);
	}
	return route;
}

/*Test(route, thread_data)
{
	char *buff;
	ri_out_connector_t* conn = mock_ri_out_connector(TYPE_FILE);
	ri_in_connector_t* in_conn = mock_ri_in_connector(TYPE_FILE);
	ri_thread_data_t* td = create_runtime_thread_data(1, in_conn, conn, 3, buff, 5);
	cr_assert(td != NULL, "create_runtime_thread_data CANNOT return a NULL value");
	cr_assert(td->data_len == 5, "data_len expected to be '5', but was:%d\n", td->data_len);
	cr_assert(td->buffer_size == 3, "buffer_size expected to be '3', but was:%d\n", td->buffer_size);
	cr_assert(td->type == TYPE_FILE, "type file expected to be TYPE_FILE, not:%d\n", td->type);
}*/

/*Test(route, conn_data)
{
	char *buff;
	ri_out_connector_t* conn = mock_ri_out_connector(TYPE_FILE);
	int max_concur = create_conn_runtime_thread_data(1, mock_ri_in_connector(TYPE_FILE), conn, 3, buff, 5);
	cr_assert(max_concur == 0, "Should have allocated the first entry, not: %d", max_concur);
}*/

/*!
  \brief Create 5 connectors, each having all thread_data containers available.
         Expected to return [0, 0, 0, 0, 0]
 */
/*Test(route, thread_data_1)
{
	char* buff;
	ri_route_t *route = mock_route(5);
	ri_connection_t* connection = (ri_connection_t*)malloc(sizeof(ri_connection_t));
	connection->route = route;
	int* containers = create_all_runtime_thread_data(connection, 1, buff, 1);
	for(int i=0; i < 5; i++){
		cr_assert(containers[i] == 0, "Expected 0 for container:%d", i);
	}
}*/

/*!
  \brief Create 5 connectors, each having all thread_data containers available.
         Expected to return [4, 3, 2, 1, 0]
 */
/*Test(route, thread_data_2)
{
	char* buff;
	ri_route_t *route = mock_route(5);
	ri_connection_t* connection = (ri_connection_t*)malloc(sizeof(ri_connection_t));
	connection->route = route;
	ri_thread_data_t* tdt = (ri_thread_data_t*)malloc(sizeof(ri_thread_data_t));
	for(int i=0; i < 5; i++){
		for(int j = 0; j < i; j++){
			route->out_connectors[i]->data[j] = tdt;
		}
	}
	int* containers = create_all_runtime_thread_data(connection, 1, buff, 1);
	for(int i=0; i < 5; i++){
		cr_assert(containers[i] == i, "Expected %d, but received:%d", i, containers[i]);
	}
}*/

/*Test(route, thread_data_3)
{
	char* buff;
	ri_route_t *route = mock_route(1);
	ri_connection_t* connection = (ri_connection_t*)malloc(sizeof(ri_connection_t));
	connection->route = route;
	ri_thread_data_t* tdt = (ri_thread_data_t*)malloc(sizeof(ri_thread_data_t));
	for(int i=0; i < MAX_ASYNC_CONCUR; i++){
		route->out_connectors[0]->data[i] = tdt;
	}
	int* containers = create_all_runtime_thread_data(connection, 1, buff, 1);
	cr_assert(containers[0] == -1, "Expected -1, but received:%d", containers[0]);
}*/


Test(route, file_conn)
{
/*	file_connector_t* conn = mock_file_connector();
	ri_file_connector_t* res = create_runtime_file_connector(conn);
	cr_assert(res != NULL, "'create_runtime_file_connector' should not return NULL");
	cr_assert(strcmp(res->filename, conn->filename)==0, "'filename' field should be copied over");*/
}

Test(route, out_conn_socket)
{
/*	out_connector_t* conn = mock_out_connector(TYPE_SOCKET);
	ri_out_connector_t* res = create_runtime_out_connector(conn);
	cr_assert(res->type == TYPE_SOCKET, "Expected TYPE_SOCKET, not:%d", res->type);
	cr_assert(res->flow == conn->flow, "Expected 'flow' to have the same value");
	for(int i=0; i < MAX_ASYNC_CONCUR; i++){
		cr_assert(res->data[i] == '\0', "data field should have been initialized");
	}*/
}
