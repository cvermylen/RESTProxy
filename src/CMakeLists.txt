include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_subdirectory(http)
add_subdirectory(socket)
add_subdirectory(frame_buffers)
add_subdirectory(logs)

set(SOURCE_FILES
  request_reply.c
  connection.c
        logs/error_handler.c
        logs/log.c
  workflow_handler.c
  route_instance.c
)
set(SOURCE_UTILS
        frame_buffers/shared_buffers.c
        socket/socket_connector.c
  file_connector.c
)

set(CMAKE_BUILD_TYPE "Debug")

add_library(gateway ${SOURCE_FILES})
add_library(buffers ${SOURCE_UTILS})
add_executable(main main.c)
target_link_libraries(main gateway buffers http stack pthread m)
