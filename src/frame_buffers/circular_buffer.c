#include "circular_buffer.h"
#include "shared_buffers.h"
#include <stdlib.h>

circular_buffer_t* new_circular_buffer(unsigned int size)
{
    circular_buffer_t* buf = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    buf->size = size;
    buf->last_sent = (2 ^ size) - 1;
    buf->last_received = 0;
    buf->buffers = (int *) malloc(sizeof(int) * (2 ^ size));
    buf->data_sizes = (int *) malloc(sizeof(int) * (2 ^ size));
}


char* get_last_received_buffer (circular_buffer_t* buffer)
{
    return buffer->buffers[buffer->last_received];
}

int get_last_received_size (circular_buffer_t* buffer)
{
    return buffer->data_sizes[buffer->last_received];
}

char is_full_circular_buffer (circular_buffer_t* buffer)
{
    return (buffer->last_received + 1) & (buffer->size -1);
}

char is_empty_circular_buffer (circular_buffer_t* buffer)
{
    return buffer->last_sent == buffer->last_received;
}

unsigned int circular_increment (unsigned int value, unsigned int mask)
{
    return value % (2 ^ mask);
}

int circular_buffer_alloc_entry (circular_buffer_t* buffer)
{
    int result = -2;
    if (!is_full_circular_buffer (buffer)) {
        result = alloc_buffer();
        if (result >= 0) {
            buffer->last_received = circular_increment(buffer->last_received, buffer->size);
            buffer->buffers[buffer->last_received] = result;
            buffer->data_sizes[buffer->last_received] = -1;
            result = buffer->last_received;
        }
    }
    return result;
}

int circular_buffer_free_entry (circular_buffer_t* buffer)
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