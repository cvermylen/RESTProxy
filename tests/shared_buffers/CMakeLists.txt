cmake_minimum_required(VERSION 3.10.0)

add_executable(circular_buffer_TEST circular_buffer_TEST.c $<TARGET_OBJECTS:circular_buffer.o>
        $<TARGET_OBJECTS:mock_shared_buffers.o>)

target_link_libraries(circular_buffer_TEST m ${CRITERION_LIBRARIES})

set_property(TARGET circular_buffer_TEST PROPERTY C_STANDARD 99)

add_test(circular_buffer_TEST bin/circular_buffer_TEST)