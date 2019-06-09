#include "circular_buffer.h"
#include "shared_buffers.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

/*! Increment the value by 1, when reaches (2 ^ mask) -1, circles back to 0
 * \private
 * @param value
 * @param mask
 * @return
 */
unsigned int circular_increment (unsigned int value, unsigned int mask)
{
    return (value + 1) & (((int)pow(2, mask)) -1);
}

/*! \private
 * decrement the value by 1, when reaches '-1', is assigned to (2 ^ mask) -1
 * @param value
 * @param mask
 * @return
 */
unsigned int circular_decrement (int value, unsigned int mask)
{
    return (value -1) & (((int)pow(2, mask)) -1);
}

circular_buffer_t* new_circular_buffer(unsigned int size, int (*feed_data) (void* conn_params, char* dest_buffer, int max_buffer_size), void* conn_params, int max_buffer_size)
{
    circular_buffer_t* buf = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    buf->size = size;
    buf->last_sent = 0;
    buf->next_to_be_received = 0;
    buf->buffers = (int *) malloc(sizeof(int) * (int)(pow(2, size)));
    for (int i=0; i < (int)(pow(2, size)); i++) {
        buf->buffers[i] = -1;
    }
    buf->data_sizes = (int *) malloc(sizeof(int) * (int)(pow(2, size)));
    for (int i=0; i < (int)(pow(2, size)); i++) {
        buf->data_sizes[i] = -1;
    }
    buf->feed_data = feed_data;
    buf->conn_params = conn_params;
    buf->max_size = max_buffer_size;
    return buf;
}


int get_last_received_buffer (circular_buffer_t* buffer)
{
    return buffer->buffers[circular_decrement(buffer->next_to_be_received, buffer->size)];
}

int get_last_received_size (circular_buffer_t* buffer)
{
    return buffer->data_sizes[circular_decrement(buffer->next_to_be_received, buffer->size)];
}

char* get_to_be_sent_buffer (circular_buffer_t* buffer)
{
    char* result = NULL;
    if (!is_empty_circular_buffer(buffer)) {
        int r = buffer->buffers[buffer->last_sent];
        result = get_buffer(r);
    }
    return result;
}

int get_to_be_sent_size (circular_buffer_t* buffer)
{
    return buffer->data_sizes[buffer->last_sent];
}

int buffer_has_been_sent (circular_buffer_t* buffer)
{
    int result = -2;
    if (!is_empty_circular_buffer (buffer)) {
        free_buffer(buffer->buffers[buffer->last_sent]);
        result = buffer->last_sent;
        buffer->buffers[buffer->last_sent] = 0;
        buffer->data_sizes[buffer->last_sent] = -1;
        buffer->last_sent = circular_increment(buffer->last_sent, buffer->size);
    }
    return result;
}

char is_full_circular_buffer (circular_buffer_t* buffer)
{
    return circular_increment(buffer->next_to_be_received, buffer->size) == buffer->last_sent;
}

char is_empty_circular_buffer (circular_buffer_t* buffer)
{
    return buffer->last_sent == buffer->next_to_be_received;
}

/*! TODO Should sleep until a circular buffer is available
 * Leave it to shared buffers to handle similar situation
 * @param buffer
 * @return
 */
int alloc_entry_in_circular_buffer (circular_buffer_t* buffer)
{
    int result = -2;
    if (!is_full_circular_buffer (buffer)) {
        result = alloc_buffer();
        if (result >= 0) {
            buffer->buffers[buffer->next_to_be_received] = result;
            buffer->data_sizes[buffer->next_to_be_received] = -1;
            buffer->next_to_be_received = circular_increment(buffer->next_to_be_received, buffer->size);
            result = buffer->next_to_be_received;
        }
    }
    return result;
}

int read_into_next_buffer(circular_buffer_t *buffer)
{
    char* dest = get_buffer(buffer->buffers[circular_decrement(buffer->next_to_be_received, buffer->size)]);
    return buffer->feed_data (buffer->conn_params, dest, buffer->max_size);
}

void set_data_size_for_last_received_buffer (circular_buffer_t* buffer, int length)
{
    buffer->data_sizes[circular_decrement(buffer->next_to_be_received, buffer->size)] = length;
}

int feed_next_buffer (circular_buffer_t* cb, circular_ptr_t* to_last_char)
{
    int result = alloc_entry_in_circular_buffer(cb);
    if (result >= 0) {
        result = read_into_next_buffer (cb);
        set_data_size_for_last_received_buffer (cb, result);
        if (result > 0) {
            to_last_char->circ_index = circular_decrement(cb->next_to_be_received, cb->size);
            to_last_char->buff_pos = cb->data_sizes[to_last_char->circ_index];
        }
    }
    return result;
}

void free_circular_buffer (circular_buffer_t* buffer)
{
    free (buffer->buffers);
    free (buffer->data_sizes);
    free (buffer);
}

void copy_circ_pointers (circular_ptr_t* to, circular_ptr_t* from)
{
    to->buff_pos = from->buff_pos;
    to->circ_index = from->circ_index;
}

int op_distance_circ_pointers(circular_buffer_t *cb, circular_ptr_t *lhs, circular_ptr_t *rhs)
{
    int result = 0;
    if (lhs->circ_index == rhs->circ_index) {
        result = rhs->buff_pos - lhs->buff_pos;
    } else {
        result = cb->data_sizes[lhs->circ_index] - lhs->buff_pos;
        int cur_index = circular_increment(lhs->circ_index, cb->size);
        while (cur_index != rhs->circ_index) {
            result += cb->data_sizes[cur_index];
            cur_index = circular_increment(cur_index, cb->size);
        }
        result += rhs->buff_pos;
    }
    return result;
}

circular_ptr_t* op_add_circ_pointers (circular_buffer_t* cb, circular_ptr_t* lhs, int ask)
{
    int remainder = ask + lhs->buff_pos;
    int idx = lhs->circ_index;
    while ((remainder = remainder - cb->data_sizes[idx]) > 0) {
        if (circular_increment(idx, cb->size) == cb->next_to_be_received) {
            //TODO: REFACTOR, as we don't necessary need this pointer.
            circular_ptr_t* raw_ptr = (circular_ptr_t*) malloc (sizeof(circular_ptr_t));
            feed_next_buffer(cb, raw_ptr);
            free (raw_ptr);
        }
        idx = circular_increment(idx, cb->size);
    }
    lhs->buff_pos = remainder + cb->data_sizes[idx];
    lhs->circ_index = idx;
    return lhs;
}

circular_ptr_t* op_sub_circ_pointer(circular_buffer_t *cb, circular_ptr_t *lhs, int ask)
{
    circular_ptr_t* result = (circular_ptr_t*) malloc (sizeof(circular_ptr_t));
    result->circ_index = lhs->circ_index;
    ask  = ask - lhs->buff_pos;
    while (ask > 0) {
        result->circ_index = circular_decrement(result->circ_index, cb->size);
        ask = ask - cb->data_sizes[result->circ_index];
    }
    result->buff_pos = ask * -1;
    return result;
}

char* get_char_ptr_from_buffer (circular_buffer_t* cb, circular_ptr_t* ptr)
{
    return &(get_buffer(cb->buffers[ptr->circ_index])[ptr->buff_pos]);
}

/*! REFACTOR The method should be replaced by a 'get_ptr_to_last_received'
 *
 */
int cmp_ptr_to_last_received (circular_buffer_t* cb, circular_ptr_t* ptr)
{
    int result = -1;
    int last_received = circular_decrement(cb->next_to_be_received, cb->size);
    if (last_received == ptr->circ_index) {
        //compare inside the buffer
        result = ptr->buff_pos - cb->data_sizes[last_received] + 1;
    }
    return result;
}

int cmp_circular_ptr (circular_buffer_t* cb, circular_ptr_t* lhs, circular_ptr_t* rhs)
{
    int result;
    if (lhs->circ_index == rhs->circ_index) {
        result = lhs->buff_pos - rhs->buff_pos;
    } else {
        int offset = (cb->last_sent > cb->next_to_be_received) ? (int)(pow(2, cb->size)) : 0;
        int lhs_circ_index = (lhs->circ_index <= cb->next_to_be_received) ? lhs->circ_index + offset : lhs->circ_index;
        int rhs_circ_index = (rhs->circ_index <= cb->next_to_be_received) ? rhs->circ_index + offset : rhs->circ_index;

        result = lhs_circ_index - rhs_circ_index;
    }
    return result;
}

static int starcount =0;
char* buffer_2_str_copy (circular_buffer_t* cb, circular_ptr_t* start, circular_ptr_t* end)
{
    printf("###DEBUG###,%d-- end ind:%d\n", starcount++, end->circ_index);
    int length = op_distance_circ_pointers(cb, start, end);
    char* result = (char*) malloc(sizeof (char) * (length +1));
    int dest_start = 0;
    int src_start = start->buff_pos;
    for (int i = start->circ_index; i != end->circ_index; i = circular_increment(i, cb->size)) {
        strncpy(&(result[dest_start]), &(get_buffer(cb->buffers[i])[src_start]), cb->data_sizes[i]- src_start);
        dest_start += cb->data_sizes[i] - src_start;
        src_start = 0;
    }
    strncpy(&(result[dest_start]), &(get_buffer(cb->buffers[end->circ_index])[src_start]), end->buff_pos- src_start);
    result[length] = '\0';
    return result;
}