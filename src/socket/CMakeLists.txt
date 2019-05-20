cmake_minimum_required(VERSION 3.1.0)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOCKET_SOURCE_FILES
        socket_connector.c socket_connector.h socket_wrap.c socket_wrap.h)

add_library(socket_conn ${SOCKET_SOURCE_FILES})
add_library(socket_connector.o OBJECT socket_connector.c)
add_library(socket_wrap.o OBJECT socket_wrap.c)
