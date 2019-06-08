# CMake generated Testfile for 
# Source directory: /home/christian/CLionProjects/RESTProxy/tests
# Build directory: /home/christian/CLionProjects/RESTProxy/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(mock_socket_connector_TEST "bin/mock_socket_connector_TEST")
add_test(shared_buffers_TEST_test "bin/shared_buffers_TEST_test")
add_test(socket_connector_TEST_test "bin/socket_connector_TEST_test")
add_test(http_first_line_TEST_test "bin/http_first_line_TEST_test")
subdirs("mocks")
subdirs("buffers")
subdirs("http")
