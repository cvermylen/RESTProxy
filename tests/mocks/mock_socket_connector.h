//
// Created by Christian Vermylen on 2019-05-14.
//

#ifndef GATEWAY_MOCK_SOCKET_CONNECTOR_H
#define GATEWAY_MOCK_SOCKET_CONNECTOR_H

void mock_socket_connect_stack_to_fd (int fd);

void mock_socket_push_buffer(int fd, char* buffer);

#endif //GATEWAY_MOCK_SOCKET_CONNECTOR_H
