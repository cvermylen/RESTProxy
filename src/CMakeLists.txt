cmake_minimum_required(VERSION 3.1.0)

project(RESTProxy)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_subdirectory(http)
add_subdirectory(socket)
add_subdirectory(buffers)
add_subdirectory(logs)

set(SOURCE_FILES
        request_reply.c
        connection.c
        logs/error_handler.c
        logs/log.c
        workflow_handler.c
        route_instance.c
        file_connector.c
)

set(CMAKE_BUILD_TYPE "Debug")


add_library(request_reply.o OBJECT request_reply.c)
add_dependencies(request_reply.o socket_connector.o)

add_library(connection.o OBJECT connection.c)
add_library(workflow_handler.o OBJECT workflow_handler.c)
add_library(route_instance.o OBJECT route_instance.c)
add_library(file_connector.o OBJECT file_connector.c)
add_library(gateway ${SOURCE_FILES})

add_executable(main main.c
        $<TARGET_OBJECTS:socket_wrap.o>)
target_link_libraries(main gateway http socket_conn buffers stack pthread rest_logs m)
