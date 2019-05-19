#include <criterion/criterion.h>
#include "../src/socket/socket_connector.h"
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

Test(socket, happy)
{
        sock_rval = 3;
        read(1, buff, 2);
        cr_assert(sock_rval == 3, "Should expect to run through 'read'");
}

Test(route, sock_conn)
{
    ri_sock_connector_t* res = create_runtime_sock_connector(1, NULL);
    cr_assert(res != NULL, "'create_runtime_sock_connector' should not return NULL");
    cr_assert(res->mode == MODE_TCP, "'mode' field should be copied over");
}
