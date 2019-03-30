include_directories(/usr/local/include)
link_directories(/usr/local/lib)
add_subdirectory(http)

set(SOURCE_FILES
  request_reply.c
  connection.c
  accept_connections.c
  error_handler.c
  log.c
  workflow_handler.c
  route_instance.c
)
set(SOURCE_UTILS
  shared_buffers.c
  socket_connector.c
  file_connector.c
)
set(CMAKE_BUILD_TYPE "Debug")
add_library(gateway ${SOURCE_FILES})
add_library(buffers ${SOURCE_UTILS})
add_executable(main main.c)
target_link_libraries(main gateway buffers stack hashmap http)
