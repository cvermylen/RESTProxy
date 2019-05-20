
#ifndef GATEWAY_CIRCULAR_BUFFER_H
#define GATEWAY_CIRCULAR_BUFFER_H

/*!
 *   Empty:
 *         |   |   |   |   |
 *               ^
 *               +---> last_sent
 *               +---> last_received
 *   Full:
 *         |   |   |   |   |
 *           ^   ^
 *           |   +---> last_sent
 *           +--->last_received
 */
typedef struct {
    unsigned int size;
    int* buffers; //Messages are transient and forwarded as they come. They are not 'bufferized' entirely.
    // managed as a circular list. Actual size will be calculated as 2 ^ ...
    int* data_sizes;  // Same size as above
    unsigned int last_sent;
    unsigned int next_to_be_received; // when last_received == last_sent -1, buffers are full
} circular_buffer_t;

/*!
 *
 * @param buffer
 * @return The buffer number. Need to be accessed with 'get_buffer'
 */
int get_last_received_buffer (circular_buffer_t* buffer);

/*!
 *
 * @param buffer
 * @return
 */
int get_last_received_size (circular_buffer_t* buffer);

/*! Initialize
 *
 * @param size Actual number of entries available = 2 ^ size
 * @return
 */
circular_buffer_t* new_circular_buffer(unsigned int size);

/*!
 * @param buffer
 * @return TRUE if the buffer is 'empty', all buffers have been sent and no new has been received.
 */
char is_empty_circular_buffer (circular_buffer_t* buffer);

/*!
 * If the circular buffer is not full, will get a new shared buffer and put it as the next entry in the circular
 * buffer. Checks as well that the shared buffer function did not return an error code.
 * @param buffer
 * @return >=0 if a new buffer is available, <0 otherwise
 */
int alloc_entry_in_circular_buffer(circular_buffer_t *buffer);

/*!
 * TODO REFACTOR. We expose this method bcse the call to the actual read is done outside of the file.
 * Maybe can be include inside the file, removing the need for this function.
 * @param buffer
 * @param length
 */
void set_data_size_for_last_received_buffer (circular_buffer_t *buffer, int length);

/*!
 * If the circular buffer is not empty, frees the entry that is supposed to have just been sent.
 * @param buffer
 * @return a negative value if the circular buffer is empty, the entry just de-allocated otherwise.
 */
int free_last_sent_in_circular_buffer(circular_buffer_t *buffer);

char* get_to_be_sent_buffer (circular_buffer_t* buffer);

int get_to_be_sent_size (circular_buffer_t* buffer);

void buffer_has_been_sent (circular_buffer_t* buffer);

void free_circular_buffer (circular_buffer_t* buffer);

#endif //GATEWAY_CIRCULAR_BUFFER_H