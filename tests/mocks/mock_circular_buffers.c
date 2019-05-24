
#include "../../src/buffers/circular_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned int circular_increment (unsigned int value, unsigned int mask)
{
    return (value + 1) & (((int)pow(2, mask)) -1);
}

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
circular_buffer_t* new_circular_buffer(unsigned int size, int fd, int (*feeder)(int fd, char* buffer, int buffer_size), int max_size)
{
    mock_called_new_circular_buffer = 1;
    return 1;
}

char** mock_shared_buffers;
circular_buffer_t* mock_create_circular_buffer (char** data, int size)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    mock_shared_buffers = (char**) malloc (sizeof(char*) * size);
    cb->buffers = (int*) malloc (sizeof(int) * size);
    cb->data_sizes = (int*) malloc (sizeof(int) * size);
    for (int i=0; i < size; i++ ) {
        mock_shared_buffers[i] = data[i];
        cb->buffers[i] = i;
        cb->data_sizes[i] = strlen (data[i]);
    }
    return cb;
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
int buffer_has_been_sent (circular_buffer_t* buffer)
{
    mock_called_buffer_has_been_sent += 1;
    return 0;
}

int mock_result_feed_next_buffer;
int feed_next_buffer (circular_buffer_t* cb, circular_ptr_t* to_last_char)
{
    return mock_result_feed_next_buffer;
}

int get_last_received_buffer (circular_buffer_t* buffer)
{
    return 0;
}


int get_last_received_size (circular_buffer_t* buffer)
{
    return 0;
}

void copy_circ_pointers (circular_ptr_t* to, circular_ptr_t* from)
{
    to->buff_pos = from->buff_pos;
    to->circ_index = from->circ_index;
}

int mock_result_op_minus_circ_pointers;
int op_distance_circ_pointers(circular_buffer_t *cb, circular_ptr_t *lhs, circular_ptr_t *rhs)
{
    return mock_result_op_minus_circ_pointers;
}

circular_ptr_t* mock_result_op_add_circ_pointers;
circular_ptr_t* op_add_circ_pointers (circular_buffer_t* cb, circular_ptr_t* lhs, int ask)
{
    if (lhs->buff_pos + ask < cb->data_sizes[lhs->circ_index]) {
        lhs->buff_pos += ask;
    } else {
        int curr_circ = lhs->circ_index;
        int t_ask = ask - (cb->data_sizes[curr_circ] - lhs->buff_pos);
        while (t_ask >= 0) {
            if (circular_increment(curr_circ, cb->size) == cb->next_to_be_received) {
                //TODO: REFACTOR, as we don't necessary need this pointer.
                circular_ptr_t* raw_ptr = (circular_ptr_t*) malloc (sizeof(circular_ptr_t));
                feed_next_buffer(cb, raw_ptr);
                free (raw_ptr);
            }
            curr_circ = circular_increment(curr_circ, cb->size);
            if (t_ask < cb->data_sizes[curr_circ]) {
                lhs->circ_index = curr_circ;
                lhs->buff_pos = t_ask;
            }
            t_ask = t_ask - cb->data_sizes[curr_circ];
        }
    }
    return lhs;
}

circular_ptr_t* mock_result_op_sub_circ_pointer;
circular_ptr_t* op_sub_circ_pointer(circular_buffer_t *cb, circular_ptr_t *lhs, int ask)
{
    return mock_result_op_sub_circ_pointer;
}

char* mock_result_get_char_ptr_from_buffer;
char* get_char_ptr_from_buffer (circular_buffer_t* cb, circular_ptr_t* ptr)
{
    return mock_result_get_char_ptr_from_buffer;
}

int mock_result_cmp_ptr_to_last_received;
int cmp_ptr_to_last_received (circular_buffer_t* cb, circular_ptr_t* ptr)
{
    return mock_result_cmp_ptr_to_last_received;
}

int mock_result_cmp_circular_ptr;
int cmp_circular_ptr (circular_buffer_t* cb, circular_ptr_t* lhs, circular_ptr_t* rhs)
{
    return mock_result_cmp_circular_ptr;
}

char* mock_result_buffer_2_str_copy;
char* buffer_2_str_copy (circular_buffer_t* cb, circular_ptr_t* start, circular_ptr_t* end)
{
    return mock_result_buffer_2_str_copy;
}