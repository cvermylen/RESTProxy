include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(SOURCE_FILES
  http_headers.c
  http_message.c
  request_reply.c
  connection.c
  accept_connections.c
  error_handler.c
  shared_buffers.c
  socket_connector.c
  log.c
  workflow_handler.c
  file_connector.c
  route_instance.c
)
add_library(gateway ${SOURCE_FILES})
add_executable(main main.c)
target_link_libraries(main gateway stack hashmap)
