cmake_minimum_required(VERSION 3.10.0)

add_executable (http_message_TEST_test http_message_TEST.c $<TARGET_OBJECTS:http_message.o>
        $<TARGET_OBJECTS:mock_first_line.o>
        $<TARGET_OBJECTS:mock_http_headers.o>
        $<TARGET_OBJECTS:mock_socket_connector.o>
        $<TARGET_OBJECTS:mock_circular_buffers.o>
        $<TARGET_OBJECTS:mock_shared_buffers.o>)
add_executable (http_headers_TEST_test http_headers_TEST.c $<TARGET_OBJECTS:http_headers.o>
        $<TARGET_OBJECTS:mock_socket_connector.o>
        $<TARGET_OBJECTS:mock_shared_buffers.o>
        $<TARGET_OBJECTS:circular_buffer.o>)
add_executable(decode_http_headers decode_http_headers.c $<TARGET_OBJECTS:http_headers.o>
        $<TARGET_OBJECTS:mock_socket_connector.o>
        $<TARGET_OBJECTS:mock_shared_buffers.o>
        $<TARGET_OBJECTS:circular_buffer.o>)

target_link_libraries (http_message_TEST_test stack m ${CRITERION_LIBRARIES})
target_link_libraries (http_headers_TEST_test stack m ${CRITERION_LIBRARIES})
target_link_libraries(decode_http_headers stack m)

set_property (TARGET http_message_TEST_test PROPERTY C_STANDARD 99)
set_property (TARGET http_headers_TEST_test PROPERTY C_STANDARD 99)

add_test (http_message_TEST_test bin/http_message_TEST_test)
add_test (http_headers_TEST_test bin/http_headers_TEST_test)