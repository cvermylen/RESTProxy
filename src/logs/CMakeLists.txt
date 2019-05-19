cmake_minimum_required(VERSION 3.1.0)

set(LOG_FILES
        error_handler.c
        log.c)

add_library(rest_logs ${LOG_FILES})