set(FRAME_BUFFERS_FILES
        shared_buffers.c)

add_library(frame_buffers shared_buffers.c circular_buffer.c circular_buffer.h)