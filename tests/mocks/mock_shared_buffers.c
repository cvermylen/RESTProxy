#include "../../src/buffers/shared_buffers.h"



int mock_called_alloc_buffer;
int mock_result_alloc_buffer;
int alloc_buffer()
{
    mock_called_alloc_buffer += 1;
    return mock_result_alloc_buffer;
}

int mock_called_get_buffer;
char* mock_result_get_buffer;
char* get_buffer(int buff_no)
{
    mock_called_get_buffer += 1;
    return mock_result_get_buffer;
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