add_subdirectory (mocks)
link_directories (lib/ /usr/local/lib)

include_directories (SYSTEM /usr/local/include/)

set (CMAKE_BUILD_TYPE "Debug")
set (CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory (buffers)
add_subdirectory (http)
add_executable (mock_socket_connector_TEST mock_socket_connector_TEST.c $<TARGET_OBJECTS:mock_socket_connector.o>
        )
add_executable (socket_connector_TEST socket_connector_TEST.c $<TARGET_OBJECTS:socket_connector_ut.o>
        $<TARGET_OBJECTS:mock_socket.o>)
add_executable(route_TEST route_TEST.c $<TARGET_OBJECTS:route.o>
        $<TARGET_OBJECTS:mock_pthread.o>
        $<TARGET_OBJECTS:mock_file_connector.o>
        $<TARGET_OBJECTS:mock_socket_connector.o>)
add_executable (route_instance_TEST route_instance_TEST.c $<TARGET_OBJECTS:route_instance.o>
        $<TARGET_OBJECTS:mock_pthread.o>
        $<TARGET_OBJECTS:mock_http_connection.o>)

target_link_libraries (mock_socket_connector_TEST stack ${CRITERION_LIBRARIES})
target_link_libraries (route_TEST stack ${CRITERION_LIBRARIES})

target_link_libraries (socket_connector_TEST stack ${CRITERION_LIBRARIES})
target_link_libraries (route_instance_TEST ${CRITERION_LIBRARIES})


set_property (TARGET mock_socket_connector_TEST PROPERTY C_STANDARD 99)
set_property (TARGET route_TEST PROPERTY C_STANDARD 99)

set_property (TARGET socket_connector_TEST PROPERTY C_STANDARD 99)
set_property (TARGET route_instance_TEST PROPERTY C_STANDARD 99)

add_test (mock_socket_connector_TEST ${PROJECT_BINARY_DIR}/bin/mock_socket_connector_TEST)
add_test (route_TEST ${PROJECT_BINARY_DIR}/bin/route_TEST)

add_test (socket_connector_TEST ${PROJECT_BINARY_DIR}/bin/socket_connector_TEST)
add_test (route_instance_TEST ${PROJECT_BINARY_DIR}/bin/route_instance_TEST)