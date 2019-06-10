cmake_minimum_required(VERSION 3.1.0)

project(RESTProxy)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_subdirectory(http)
add_subdirectory(socket)
add_subdirectory(buffers)
add_subdirectory(logs)
add_subdirectory(thread)

set(SOURCE_FILES
        logs/error_handler.c
        logs/log.c
        workflow_handler.c
        route_instance.c
        file_connector.c
        route.c
        route_instance.c)

set(CMAKE_BUILD_TYPE "Debug")

add_library(workflow_handler.o OBJECT workflow_handler.c)
add_library(route_instance.o OBJECT route_instance.c)
add_library(route.o OBJECT route.c)
add_library(file_connector.o OBJECT file_connector.c)
add_library(gateway ${SOURCE_FILES})

add_executable(main main.c
        $<TARGET_OBJECTS:socket_wrap.o>)
target_link_libraries(main http gateway socket_conn buffers stack pthread pthread_wrap rest_logs m)
