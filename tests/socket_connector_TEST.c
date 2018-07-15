#include <criterion/criterion.h>
#include "../src/socket_connector.h"
#include "../src/route_instance.h"
#include "../src/route_def.h"
#include "../src/request_reply.h"
#include "mocks/mock_socket.h"

int has_been_called;

char buff[3];

void mock_file_callback(void *params)
{
	has_been_called = 1;
}

Test(file, push_2_file)
{
	int fake_fd = 3;
	reply_t *con = (reply_t*)malloc(sizeof(reply_t));
	con->content.file = (ri_file_connector_t*)malloc(sizeof(ri_file_connector_t));
	con->type = TYPE_FILE;
	con->request = (request_t*)malloc(sizeof(request_t));
	con->content.file->file = (FILE*)&fake_fd;
	con->content.file->output_callback = mock_file_callback;
	con->request->http_message = (http_message_t*)malloc(sizeof(http_message_t));
	con->request->http_message->buffer = (char*)malloc(sizeof(char));
	con->request->http_message->buffer[0] = '\0';
	has_been_called = 0;
	push_data_2_destination(con);

	cr_assert(has_been_called == 1, "Callback function 'mock_file_callback' should have been called");
}

Test(socket, happy)
{
        sock_rval = 3;
        read(1, buff, 2);
        cr_assert(sock_rval == 3, "Should expect to run through 'read'");
}
