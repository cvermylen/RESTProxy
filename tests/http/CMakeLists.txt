cmake_minimum_required(VERSION 3.10.0)

add_executable (http_message_TEST_test http_message_TEST.c $<TARGET_OBJECTS:http_message.o>
        $<TARGET_OBJECTS:mock_first_line.o> $<TARGET_OBJECTS:mock_http_headers.o>
        $<TARGET_OBJECTS:mock_socket_connector.o>
        $<TARGET_OBJECTS:mock_circular_buffers.o>
        $<TARGET_OBJECTS:mock_shared_buffers.o>)

target_link_libraries (http_message_TEST_test stack ${CRITERION_LIBRARIES})

set_property (TARGET http_message_TEST_test PROPERTY C_STANDARD 99)

add_test (http_message_TEST_test bin/http_message_TEST_test)