include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_library(mock_socket mock_socket.c mock_log.c)
add_library(mock_buffers mock_file_connector.c mock_buffers.c)
add_library(mock_socket_connector mock_socket_connector.c)
