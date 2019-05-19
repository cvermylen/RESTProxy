
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
    unsigned int last_received; // when last_received == last_sent -1, buffers are full
} circular_buffer_t;


/*!
 *
 * @param size Actual number of entries available = 2 ^ size
 * @return
 */
circular_buffer_t* new_circular_buffer(unsigned int size);

/*!
 * If the circular buffer is not full, will get a new shared buffer and put it as the next entry in the circular
 * buffer. Checks as well that the shared buffer function did not return an error code.
 * @param buffer
 * @return >=0 if a new buffer is available, <0 otherwise
 */
int circular_buffer_alloc_entry (circular_buffer_t* buffer);

/*!
 * If the circular buffer is not empty, frees the entry that is supposed to have just been sent.
 * @param buffer
 * @return a negative value if the circular buffer is empty, the entry just de-allocated otherwise.
 */
int circular_buffer_free_entry (circular_buffer_t* buffer);


#endif //GATEWAY_CIRCULAR_BUFFER_H