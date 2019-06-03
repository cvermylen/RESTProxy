
#ifndef GATEWAY_CIRCULAR_BUFFER_H
#define GATEWAY_CIRCULAR_BUFFER_H

/*!
 *  If n is the number of entries in the circular buffer, n - 1 is the maximum capacity.
 *  Example, let the size = 3, the buffer size is (2 ^ 3) = 8, minus one = 7 entries that can be used.
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

    int (*feed_data) (void* conn_params, char* dest_buffer, int max_buffer_size); //where void* is ri_sock_connector_t...
    void* conn_params;
    int max_size;
} circular_buffer_t;

/*! Set of pointers to point to data inside a circular buffer
 */
typedef  struct {
    int circ_index; //index of circular buffer
    int buff_pos;   //position into the buffer pointed to by circ_index
} circular_ptr_t;

unsigned int circular_increment (unsigned int value, unsigned int mask);

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
circular_buffer_t* new_circular_buffer(unsigned int size, int (*feed_data) (void* conn_params, char* dest_buffer, int max_buffer_size), void* conn_params, int max_buffer_size);

/*!
 * @param buffer
 * @return TRUE if the buffer is 'empty', all buffers have been sent and no new has been received.
 */
char is_empty_circular_buffer (circular_buffer_t* buffer);

/*!
 * If the circular buffer is not full, will get a new shared buffer and put it as the next entry in the circular
 * buffer. Checks as well that the shared buffer function did not return an error code.
 * @param buffer
 * @return circular buffer entry, >=0 if a new buffer is available, <0 otherwise
 */
int alloc_entry_in_circular_buffer(circular_buffer_t *buffer);

/*! Get data from the source and dump it into the next buffer.
 * @param buffer
 * @return The size of the data collected
 */
int read_into_next_buffer(circular_buffer_t *buffer);

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

/*! The size of the actual data in the next buffer on the list to be sent.
 * @param buffer
 * @return
 */
int get_to_be_sent_size (circular_buffer_t* buffer);

/*! Mark the 'last_send' buffer as 'sent', freeing the entry and moving as well to the next buffer for sending.
 * @param buffer
 */
int buffer_has_been_sent (circular_buffer_t* buffer);

/*! Allocate a new buffer, and fill it with data
 * @param buffer
 * @return the number of new characters read
 */
int feed_next_buffer (circular_buffer_t* cb, circular_ptr_t* to_last_char);

void free_circular_buffer (circular_buffer_t* buffer);

/*! Copy the content of one pointer set to the destination
 * @param to
 * @param from
 */
void copy_circ_pointers (circular_ptr_t* to, circular_ptr_t* from);

/*! Calculates the distance between the 2 pointer sets. 'lhs' is supposed to be 'before' 'rhs',
 * otherwise the result is undefined. The distance is the number of characters, from and including lhs to
 * and excluding rhs: 3(rhs) - 1(lhs) = 2.
 * @param cb
 * @param lhs
 * @param rhs
 * @return number of characters between the 2 pointers sets
 */
int op_distance_circ_pointers(circular_buffer_t *cb, circular_ptr_t *lhs, circular_ptr_t *rhs);

/*! Add the number asked to the pointer set passed in parameter. Load data if necessary
 * @param cb
 * @param lhs
 * @param ask
 */
circular_ptr_t* op_add_circ_pointers (circular_buffer_t* cb, circular_ptr_t* lhs, int ask);

/*! Subtract the number asked to the pointer set passed in parameter.
 * @param cb
 * @param lhs
 * @param ask
 */
circular_ptr_t* op_sub_circ_pointer(circular_buffer_t *cb, circular_ptr_t *lhs, int ask);
/*! Returns the character pointed. The pointer must be in the range of the loaded buffers.
 * The method does not indicates the upper bound boundary of the returned string.
 * TODO REFACTOR: creates a method that returns 'safely' one or 2 characters.
 * @param cb
 * @param ptr
 * @return a newly allocated circular pointer
 */
char* get_char_ptr_from_buffer (circular_buffer_t* cb, circular_ptr_t* ptr);

/*! Compares the pointer to the position of the last character received
 * @return -1, 0, 1 if the ptr is before, equal or after the last character received
 */
int cmp_ptr_to_last_received (circular_buffer_t* cb, circular_ptr_t* ptr);

int cmp_circular_ptr (circular_buffer_t* cb, circular_ptr_t* lhs, circular_ptr_t* rhs);

/*! Extract in a single string the content between start and end
 * @param cb
 * @param start
 * @param end
 * @return newly malloc'd string
 */
char* buffer_2_str_copy (circular_buffer_t* cb, circular_ptr_t* start, circular_ptr_t* end);

#endif //GATEWAY_CIRCULAR_BUFFER_H