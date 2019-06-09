add_subdirectory (mocks)
link_directories (lib/ /usr/local/lib)

include_directories (SYSTEM /usr/local/include/)

set (CMAKE_BUILD_TYPE "Debug")
set (CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory (buffers)
add_subdirectory (http)
add_executable (mock_socket_connector_TEST mock_socket_connector_TEST.c $<TARGET_OBJECTS:mock_socket_connector.o>
        )
#add_executable (connections_TEST connections_TEST.c $<TARGET_OBJECTS:connection.o>
#        $<TARGET_OBJECTS:mock_shared_buffers.o>
#        $<TARGET_OBJECTS:mock_socket_connector.o>
#        $<TARGET_OBJECTS:mock_socket.o>
#        $<TARGET_OBJECTS:mock_circular_buffers.o>)

#  $<TARGET_OBJECTS:mock_pthread.o>

# $<TARGET_OBJECTS:mock_socket.o>
add_executable (socket_connector_TEST_test socket_connector_TEST.c $<TARGET_OBJECTS:socket_connector_ut.o>
        $<TARGET_OBJECTS:mock_socket.o>)
#add_executable (route_instance_TEST_test route_instance_TEST.c $<TARGET_OBJECTS:route_instance.o>)

add_executable (http_first_line_TEST_test http_first_line_TEST.c $<TARGET_OBJECTS:http_first_line.o>
        $<TARGET_OBJECTS:mock_shared_buffers.o>)

target_link_libraries (mock_socket_connector_TEST stack ${CRITERION_LIBRARIES})
#target_link_libraries (connections_TEST stack ${CRITERION_LIBRARIES})

target_link_libraries (socket_connector_TEST_test stack ${CRITERION_LIBRARIES})
#target_link_libraries (route_instance_TEST_test ${CRITERION_LIBRARIES})

target_link_libraries (http_first_line_TEST_test ${CRITERION_LIBRARIES})


set_property (TARGET mock_socket_connector_TEST PROPERTY C_STANDARD 99)
#set_property (TARGET connections_TEST PROPERTY C_STANDARD 99)

set_property (TARGET socket_connector_TEST_test PROPERTY C_STANDARD 99)
#set_property (TARGET route_instance_TEST_test PROPERTY C_STANDARD 99)

set_property (TARGET http_first_line_TEST_test PROPERTY C_STANDARD 99)

add_test (mock_socket_connector_TEST bin/mock_socket_connector_TEST)
#add_test (connections_TEST bin/connections_TEST)

add_test (socket_connector_TEST_test bin/socket_connector_TEST_test)
#add_test (route_instance_TEST_test bin/route_instance_TEST_test)

add_test (http_first_line_TEST_test bin/http_first_line_TEST_test)
