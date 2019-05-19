
#include "../../src/frame_buffers/circular_buffer.h"
#include <stdlib.h>

//Circular buffers
int mock_called_set_data_size_for_last_received_buffer;
void set_data_size_for_last_received_buffer (circular_buffer_t *buffer, int length)
{
    mock_called_set_data_size_for_last_received_buffer = 1;
}

int mock_called_free_circular_buffer;
void free_circular_buffer (circular_buffer_t* buffer)
{
    mock_called_free_circular_buffer = 1;
}

int mock_called_new_circular_buffer;
circular_buffer_t* new_circular_buffer(unsigned int size)
{
    mock_called_new_circular_buffer = 1;
    return 1;
}

int mock_called_alloc_entry_in_circular_buffer;
int mock_result_alloc_entry_in_circular_buffer;
int alloc_entry_in_circular_buffer (circular_buffer_t *buffer)
{
    mock_called_alloc_entry_in_circular_buffer += 1;
    return mock_result_alloc_entry_in_circular_buffer;
}

int mock_called_free_last_sent_in_circular_buffer;
int mock_result_free_last_sent_in_circular_buffer;
int free_last_sent_in_circular_buffer(circular_buffer_t *buffer)
{
    mock_called_free_last_sent_in_circular_buffer += 1;
    return mock_result_free_last_sent_in_circular_buffer;
}

int mock_called_is_empty_circular_buffer;
char mock_result_is_empty_circular_buffer;
char is_empty_circular_buffer (circular_buffer_t* buffer)
{
    mock_called_is_empty_circular_buffer += 1;
    return mock_result_is_empty_circular_buffer;
}

int mock_called_get_to_be_sent_buffer;
char* mock_result_get_to_be_sent_buffer;
char* get_to_be_sent_buffer (circular_buffer_t* buffer) {
    mock_called_get_to_be_sent_buffer += 1;
    return mock_result_get_to_be_sent_buffer;
}

int mock_called_get_to_be_sent_size;
int mock_result_get_to_be_sent_size;
int get_to_be_sent_size (circular_buffer_t* buffer)
{
    mock_called_get_to_be_sent_size += 1;
    return mock_result_get_to_be_sent_size;
}

int mock_called_buffer_has_been_sent;
void buffer_has_been_sent (circular_buffer_t* buffer)
{
    mock_called_buffer_has_been_sent += 1;
}


int get_last_received_buffer (circular_buffer_t* buffer)
{
    return 0;
}


int get_last_received_size (circular_buffer_t* buffer)
{
    return 0;
}
