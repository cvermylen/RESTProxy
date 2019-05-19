cmake_minimum_required(VERSION 3.1.0)

set(SOCKET_SOURCE_FILES
        socket_connector.c)

add_library(socket_conn ${SOCKET_SOURCE_FILES})
add_library(socket_connector.o OBJECT socket_connector.c)