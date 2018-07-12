#ifndef ACCEPT_CONNECTIONS
#define ACCEPT_CONNECTIONS
#include <netinet/in.h>
#include "socket_connector.h"
#include "route_instance.h"

#define BINDING_ERROR	-20000

int bind_port(const int portno);
ri_route_t* wait_4_connection_request(rt_route_t *route);

/*!
  \brief close the file descriptor associated to the described connection, and free the memory allocated to the descriptor
 */
void close_connection(ri_route_t *c);
#endif
