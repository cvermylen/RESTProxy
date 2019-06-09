cmake_minimum_required(VERSION 3.1.0)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOCKET_SOURCE_FILES
        pthread_wrap.c pthread_wrap.h)

add_library(pthread_wrap ${SOCKET_SOURCE_FILES})
add_library(pthread_wrap.o OBJECT pthread_wrap.c)
