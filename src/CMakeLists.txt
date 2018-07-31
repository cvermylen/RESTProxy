include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(SOURCE_FILES
  http_headers.c
  http_message.c
  request_reply.c
  connection.c
  accept_connections.c
  error_handler.c
  log.c
  workflow_handler.c
  route_instance.c
  http_first_line.c
)
set(SOURCE_UTILS
  shared_buffers.c
  socket_connector.c
  file_connector.c
)
add_library(gateway ${SOURCE_FILES})
add_library(buffers ${SOURCE_UTILS})
add_executable(main main.c)
target_link_libraries(main gateway buffers stack hashmap)
