cmake_minimum_required(VERSION 3.10.0)

add_executable (http_message_TEST http_message_TEST.c $<TARGET_OBJECTS:http_message.o>
        $<TARGET_OBJECTS:mock_http_first_line.o>
        $<TARGET_OBJECTS:mock_http_headers.o>
        $<TARGET_OBJECTS:mock_socket_connector.o>
        $<TARGET_OBJECTS:mock_circular_buffers.o>
        $<TARGET_OBJECTS:mock_shared_buffers.o>)

add_executable (http_headers_TEST http_headers_TEST.c $<TARGET_OBJECTS:http_headers.o>
        $<TARGET_OBJECTS:mock_socket_connector.o>
        $<TARGET_OBJECTS:mock_shared_buffers.o>
        $<TARGET_OBJECTS:circular_buffer.o>)

add_executable(decode_http_headers decode_http_headers.c $<TARGET_OBJECTS:http_headers.o>
        $<TARGET_OBJECTS:mock_socket_connector.o>
        $<TARGET_OBJECTS:mock_shared_buffers.o>
        $<TARGET_OBJECTS:circular_buffer.o>)

add_executable (http_first_line_TEST http_first_line_TEST.c $<TARGET_OBJECTS:http_first_line.o>
        $<TARGET_OBJECTS:mock_shared_buffers.o>)

add_executable (http_request_TEST http_request_TEST.c $<TARGET_OBJECTS:http_request.o>
        $<TARGET_OBJECTS:mock_http_message.o>
        $<TARGET_OBJECTS:mock_http_headers.o>)

add_executable(http_reply_TEST http_reply_TEST.c $<TARGET_OBJECTS:http_reply.o>
        $<TARGET_OBJECTS:mock_http_message.o>
        $<TARGET_OBJECTS:mock_pthread.o>)

add_executable (request_replies_TEST request_replies_TEST.c $<TARGET_OBJECTS:request_replies.o>
        $<TARGET_OBJECTS:mock_http_request.o>
        $<TARGET_OBJECTS:mock_http_reply.o>
        $<TARGET_OBJECTS:mock_http_message.o>
        $<TARGET_OBJECTS:mock_pthread.o>)

add_executable (http_connection_TEST http_connection_TEST.c $<TARGET_OBJECTS:http_connection.o>
        $<TARGET_OBJECTS:mock_request_replies.o>
        $<TARGET_OBJECTS:mock_http_request.o>
        $<TARGET_OBJECTS:mock_http_reply.o>)

target_link_libraries (http_message_TEST stack m ${CRITERION_LIBRARIES})
target_link_libraries (http_headers_TEST stack m ${CRITERION_LIBRARIES})
target_link_libraries(decode_http_headers stack m ${CRITERION_LIBRARIES})
target_link_libraries(http_request_TEST ${CRITERION_LIBRARIES})
target_link_libraries(http_reply_TEST pthread ${CRITERION_LIBRARIES})
target_link_libraries (http_first_line_TEST ${CRITERION_LIBRARIES})
target_link_libraries (request_replies_TEST ${CRITERION_LIBRARIES})
target_link_libraries(http_connection_TEST stack ${CRITERION_LIBRARIES})

set_property (TARGET http_message_TEST PROPERTY C_STANDARD 99)
set_property (TARGET http_headers_TEST PROPERTY C_STANDARD 99)
set_property(TARGET http_request_TEST PROPERTY C_STANDARD 99)
set_property(TARGET http_reply_TEST PROPERTY C_STANDARD 99)
set_property (TARGET http_first_line_TEST PROPERTY C_STANDARD 99)
set_property (TARGET request_replies_TEST PROPERTY C_STANDARD 99)
set_property(TARGET http_connection_TEST PROPERTY C_STANDARD 99)

add_test (http_message_TEST ${PROJECT_BINARY_DIR}/bin/http_message_TEST)
add_test (http_headers_TEST ${PROJECT_BINARY_DIR}/bin/http_headers_TEST)
add_test (http_request_TEST ${PROJECT_BINARY_DIR}/bin/http_request_TEST)
add_test (http_reply_TEST ${PROJECT_BINARY_DIR}/bin/http_reply_TEST)
add_test (http_first_line_TEST ${PROJECT_BINARY_DIR}/bin/http_first_line_TEST)
add_test (request_replies_TEST ${PROJECT_BINARY_DIR}/bin/request_replies_TEST)
add_test (http_connection_TEST ${PROJECT_BINARY_DIR}/bin/http_connection_TEST)