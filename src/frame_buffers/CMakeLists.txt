cmake_minimum_required(VERSION 3.1.0)

set(BUFFER_SOURCE_FILES
        shared_buffers.c
        circular_buffer.c)

add_library(buffers ${BUFFER_SOURCE_FILES})

add_library(shared_buffers.o OBJECT shared_buffers.c)
add_library(circular_buffer.o OBJECT circular_buffer.c)