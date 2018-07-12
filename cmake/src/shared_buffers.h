#ifndef SHARED_BUFFERS_H
#define SHARED_BUFFERS_H

#define TX_BUFFER_SIZE	4096
#define MAX_BUFFERS	4096

/*! 
  \brief Initial number of buffers is set to MAX_BUFFER, unless overriden by a specific value.
         Call to this method must be done before calling init_shared_buffers.
  \param max_buffers number of buffers available
 */
void override_max_buffers(const int max_buffers);
void override_tx_buffer_size(const int size);

/*!
  \brief Must be called once before allocating/releasing any buffer.
 */
void init_shared_buffers();

/*!
  \brief Returns the buffer pointed to by the key
 */
char* get_buffer(const int key);

/*!
  \brief returns a key to the free buffer, or -1 if no buffer is available
 */
extern int alloc_buffer();

/*!
  \brief Returns the number of buffers that can still be allocated.
 */
extern int available_buffers();

/*!
  \brief Returns the buffer to the free pool.
 */
void free_buffer(const int key);

#endif
