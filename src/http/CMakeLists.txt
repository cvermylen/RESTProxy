cmake_minimum_required(VERSION 3.1.0)

set(HTTP_SOURCE_FILES
  http_first_line.c
  http_headers.c
  http_message.c
        )

add_library(http ${HTTP_SOURCE_FILES})
add_library(http_message.o OBJECT http_message.c)
add_library(http_first_line.o OBJECT http_first_line.c)
add_library(http_headers.o OBJECT http_headers.c)