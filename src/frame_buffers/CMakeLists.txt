cmake_minimum_required(VERSION 3.1.0)

add_library(frame_buffers shared_buffers.c circular_buffer.c)
add_library(shared_buffers.o OBJECT shared_buffers.c)
add_library(circular_buffer.c OBJECT circular_buffer.c)