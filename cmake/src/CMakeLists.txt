include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_library(gateway accept_connections.c error_handler.c shared_buffers.c socket_io.c routing_params.c socket_connector.c routing_handler.c log.c workflow_handler.c file_connector.c route_instance.c)
add_executable(main main.c)
target_link_libraries(main gateway stack)
