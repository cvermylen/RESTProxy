#include "../../src/buffers/shared_buffers.h"

#include <stdio.h>
#include <stdio.h>

int mock_called_alloc_buffer;
int mock_result_alloc_buffer;
int alloc_buffer()
{
    if (mock_result_alloc_buffer != 0)
        return mock_result_alloc_buffer;
    mock_called_alloc_buffer += 1;
    return mock_called_alloc_buffer;
}

int mock_called_get_buffer;
char* mock_result_get_buffer[500];
int mock_param1_get_buffer;
char* get_buffer(int buff_no)
{
    mock_called_get_buffer += 1;
    mock_param1_get_buffer = buff_no;
    printf("Called get_buffer with param:%d\n", buff_no);
    return mock_result_get_buffer[buff_no];
}

int mock_called_free_buffer;
void free_buffer(const int key)
{
    mock_called_free_buffer += 1;
}

void init_mock_shared_buffers_variables ()
{
    mock_called_alloc_buffer = 0;
    mock_called_get_buffer = 0;
    mock_called_free_buffer = 0;
}