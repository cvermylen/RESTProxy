#include "../../src/frame_buffers/shared_buffers.h"

char* internal_mock_buffer;

int alloc_buffer()
{
    return 1;
}

char* get_buffer(int buff_no)
{
    return internal_mock_buffer;
}

void free_buffer(const int key)
{
}

