# CMake generated Testfile for 
# Source directory: /Users/christian/projects/RESTProxy/tests
# Build directory: /Users/christian/projects/RESTProxy/src/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(accept_connections_TEST "accept_connections_TEST")
add_test(shared_buffers_TEST_test "shared_buffers_TEST_test")
add_test(socket_connector_TEST_test "socket_connector_TEST_test")
add_test(route_instance_TEST_test "route_instance_TEST_test")
add_test(http_headers_TEST_test "http_headers_TEST_test")
add_test(http_message_TEST_test "http_message_TEST_test")
add_test(http_first_line_TEST_test "http_first_line_TEST_test")
subdirs("mocks")
