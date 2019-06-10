cmake_minimum_required(VERSION 3.10.0)

set(HTTP_SOURCE_FILES
        http_first_line.c
        http_headers.c
        http_message.c
        http_request.c
        http_reply.c
        http_connection.c
        request_replies.c)

add_library(http ${HTTP_SOURCE_FILES})

add_library(http_message.o OBJECT http_message.c)
add_library(http_first_line.o OBJECT http_first_line.c)
add_library(http_headers.o OBJECT http_headers.c)
add_library(http_request.o OBJECT http_request.c)
add_library(http_reply.o OBJECT http_reply.c)
add_library(http_connection.o OBJECT http_connection.c)
add_library(request_replies.o OBJECT request_replies.c)