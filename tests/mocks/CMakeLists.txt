include_directories(/usr/local/include)
#link_directories(/usr/local/lib)

add_library(mock_http_message.o OBJECT mock_http_message.c)
add_library(mock_socket.o OBJECT mock_socket.c)
add_library(mock_file_connector.o OBJECT mock_file_connector.c)
add_library(mock_socket_connector.o OBJECT mock_socket_connector.c)
add_library(mock_http_headers.o OBJECT mock_http_headers.c)
add_library(mock_http_first_line.o OBJECT mock_http_first_line.c)
add_library(mock_shared_buffers.o OBJECT mock_shared_buffers.c)
add_library(mock_circular_buffers.o OBJECT mock_circular_buffers.c)
add_library(mock_pthread.o OBJECT mock_pthread.c)
add_library(mock_http_request.o OBJECT mock_http_request.c)
add_library(mock_http_reply.o OBJECT mock_http_reply.c)
add_library(mock_request_replies.o OBJECT mock_request_replies.c)

add_library(socket_connector_ut.o OBJECT ../../src/socket/socket_connector.c)

target_compile_definitions(socket_connector_ut.o PUBLIC MOCK_CALLS)
