add_subdirectory(mocks)
link_directories(/usr/local/lib)

include_directories(SYSTEM /usr/local/include/)

set(CMAKE_BUILD_TYPE "Debug")

add_executable(accept_connections_TEST accept_connections_TEST.c)
add_executable(shared_buffers_TEST_test shared_buffers_TEST.c)
add_executable(socket_connector_TEST_test socket_connector_TEST.c)
add_executable(route_instance_TEST_test route_instance_TEST.c)
add_executable(http_headers_TEST_test http_headers_TEST.c)
add_executable(http_message_TEST_test http_message_TEST.c)

target_link_libraries(accept_connections_TEST gateway mock_gateway ${CRITERION_LIBRARIES})
target_link_libraries(shared_buffers_TEST_test gateway ${CRITERION_LIBRARIES})
target_link_libraries(socket_connector_TEST_test gateway stack mock_gateway ${CRITERION_LIBRARIES})
target_link_libraries(route_instance_TEST_test gateway stack ${CRITERION_LIBRARIES})
target_link_libraries(http_headers_TEST_test gateway stack ${CRITERION_LIBRARIES})
target_link_libraries(http_message_TEST_test mock_buffers gateway stack ${CRITERION_LIBRARIES})

set_property (TARGET accept_connections_TEST PROPERTY C_STANDARD 99)
set_property (TARGET shared_buffers_TEST_test PROPERTY C_STANDARD 99)
set_property (TARGET socket_connector_TEST_test PROPERTY C_STANDARD 99)
set_property (TARGET route_instance_TEST_test PROPERTY C_STANDARD 99)
set_property (TARGET http_headers_TEST_test PROPERTY C_STANDARD 99)
set_property (TARGET http_message_TEST_test PROPERTY C_STANDARD 99)

add_test(accept_connections_TEST accept_connections_TEST)
add_test(shared_buffers_TEST_test shared_buffers_TEST_test)
add_test(socket_connector_TEST_test socket_connector_TEST_test)
add_test(route_instance_TEST_test route_instance_TEST_test)
add_test(http_headers_TEST_test http_headers_TEST_test)
add_test(http_message_TEST_test http_message_TEST_test)
