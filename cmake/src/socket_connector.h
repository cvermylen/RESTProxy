#ifndef SOCKET_CONNECTOR_H
#define SOCKET_CONNECTOR_H
#include <pthread.h>
#include <stdio.h>
#include <netinet/in.h>
#include "routing_handler.h"

void* push_data_2_destination(void* params);
void* socket_connector(void* ptr_fd);

#endif
