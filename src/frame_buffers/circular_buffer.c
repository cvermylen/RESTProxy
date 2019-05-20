#include "circular_buffer.h"
#include "shared_buffers.h"
#include <stdlib.h>
#include <math.h>

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

circular_buffer_t* new_circular_buffer(unsigned int size)
{
    circular_buffer_t* buf = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    buf->size = size;
    buf->last_sent = 0;
    buf->next_to_be_received = 0;
    buf->buffers = (int *) malloc(sizeof(int) * (pow(2, size)));
    buf->data_sizes = (int *) malloc(sizeof(int) * (pow(2, size)));
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
    return buffer->buffers[buffer->last_sent];
}

void buffer_has_been_sent (circular_buffer_t* buffer)
{
    buffer->buffers[buffer->last_sent] = 0;
    buffer->data_sizes[buffer->last_sent] = -1;
    buffer->last_sent = circular_increment (buffer->last_sent, buffer->size);
}

char is_full_circular_buffer (circular_buffer_t* buffer)
{
    return circular_increment(buffer->next_to_be_received, buffer->size) == buffer->last_sent;
}

char is_empty_circular_buffer (circular_buffer_t* buffer)
{
    return buffer->last_sent == buffer->next_to_be_received;
}


int alloc_entry_in_circular_buffer (circular_buffer_t *buffer)
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

void set_data_size_for_last_received_buffer (circular_buffer_t *buffer, int length)
{
    buffer->data_sizes[circular_decrement(buffer->next_to_be_received, buffer->size)] = length;
}

int free_last_sent_in_circular_buffer(circular_buffer_t *buffer)
{
    int result = -2;
    if (!is_empty_circular_buffer (buffer)) {
        free_buffer (buffer->buffers[buffer->last_sent]);
        buffer->data_sizes[buffer->last_sent] = -1;
        result = buffer->last_sent;
        buffer->last_sent = circular_increment(buffer->last_sent, buffer->size);
    }
    return result;
}

void free_circular_buffer (circular_buffer_t* buffer)
{
    free (buffer->buffers);
    free (buffer->data_sizes);
    free (buffer);
}