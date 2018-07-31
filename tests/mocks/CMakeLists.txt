include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_library(mock_gateway mock_socket.c mock_log.c)
add_library(mock_buffers mock_file_connector.c mock_socket_connector.c mock_buffers.c)
