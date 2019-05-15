#include "../src/socket/socket_connector.h"
#include "mocks/mock_socket_connector.h"
#include <str_stack.h>
#include <criterion/criterion.h>
#include <stdlib.h>

Test(mock_socket, null)
{
    int r = read_from_socket(0, NULL, 0);

    cr_assert(-1 == r, "Stack in mock_socket is NULL, return is expected to return '-1', not:%d", r);
}

Test(mock_socket, empty)
{
    mock_socket_connect_stack_to_fd(0);

    int r = read_from_socket(0, NULL, 0);

    cr_assert(-1 == r, "Stack in mock_socket is empty, return is expected to return '-1', not:%d", r);
}

Test(mock_socket, one_line)
{
    mock_socket_connect_stack_to_fd(1);
    char buffer[] = "A simple string";
    mock_socket_push_buffer(1, buffer);
    char* result = (char*) malloc(sizeof(char) * 50);

    int r = read_from_socket(1, result, 50);

    cr_assert(strlen(buffer) == r, "Stack is not empty, expected to return length of the string, not:%d", r);
}

Test(mock_socket, two_lines)
{
    mock_socket_connect_stack_to_fd(2);
    char buffer2[] = "Second line";
    char buffer1[] = "First line";
    mock_socket_push_buffer(2, buffer2);
    mock_socket_push_buffer(2, buffer1);
    char* result = (char*) malloc(sizeof(char) * 50);

    int r1 = read_from_socket(2, result, 50);
    cr_assert(strcmp(buffer1, result) == 0, "Expected to return %s, not: %s", buffer1, result);

    int r2 = read_from_socket(2, result, 50);
    cr_assert(strcmp(buffer2, result) == 0, "Expected to return %s, not: %s", buffer2, result);
}