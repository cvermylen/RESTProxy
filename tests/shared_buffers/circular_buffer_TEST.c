#include <criterion/criterion.h>
#include "../../src/frame_buffers/circular_buffer.h"
#include <math.h>
#include <signal.h>

#include <stdio.h>

extern void init_mock_shared_buffers_variables ();
extern unsigned int circular_decrement (int value, unsigned int mask);
Test (decrement, zero)
{
    unsigned int r = circular_decrement (0, 0);
    cr_assert(0 == r, "With buffer of size 1, index should always be 0, not:%u", r);
}

Test (decrement, 0_to_7)
{
    unsigned int r = circular_decrement (0, 3);
    cr_assert(7 == r, "Was expecting  (2 ^ 3) - 1 = 7, not:%u", r);
}

Test (decrement, 1_to_0)
{
    unsigned int r = circular_decrement(1, 3);
    cr_assert(0 == r, "Size of the buffer should not matter (expected 0). Got:%u", r);
}

Test (decrement, 7_to_6)
{
    unsigned int r = circular_decrement(7, 3);
    cr_assert(6 == r, "Size of the buffer should not matter (expected 6). Got:%u", r);
}

Test (decrement, 9_to_0)
{
    unsigned int r = circular_decrement(9, 3);
    cr_assert(0 == r, "Out of bound value should be constrained, expecting 0, not:%u", r);
}

Test (decrement, 8_to_7)
{
    unsigned int r = circular_decrement(8, 3);
    cr_assert(7 == r, "Out of bound value should be constrained, expecting 7, not:%u", r);
}

extern unsigned int circular_increment (int value, unsigned int mask);
Test (increment, zero)
{
    unsigned int r = circular_decrement (0, 0);
    cr_assert(0 == r, "With buffer of size 1, index should always be 0, not:%u", r);
}

Test (increment, 7_to_0)
{
    unsigned int r = circular_increment (7, 3);
    cr_assert(0 == r, "Was expecting 0, not:%u", r);
}

Test (increment, 0_to_1)
{
    unsigned int r = circular_increment(0, 3);
    cr_assert(1 == r, "Size of the buffer should not matter (expected 1). Got:%u", r);
}

Test (increment, 6_to_7)
{
    unsigned int r = circular_increment(6, 3);
    cr_assert(7 == r, "Size of the buffer should not matter (expected 7). Got:%u", r);
}

Test (increment, 9_to_0)
{
    unsigned int r = circular_increment(9, 3);
    cr_assert(2 == r, "Out of bound value should be constrained, expecting 2, not:%u", r);
}

Test (increment, 8_to_7)
{
    unsigned int r = circular_increment(8, 3);
    cr_assert(1 == r, "Out of bound value should be constrained, expecting 1, not:%u", r);
}

Test (circular_buffer, new)
{
    circular_buffer_t* cb = new_circular_buffer(3);

    cr_assert_not_null(cb, "new method expected to return something");
    cr_assert(3 == cb->size, "The 'size' property should be initialized at 3, not:%d", cb->size);

    free_circular_buffer(cb);
}


Test (circular_buffer, buffer_access_ok)
{
    circular_buffer_t* cb = new_circular_buffer(3);

    for (int i = 0; i < 8; i++) {
        cb->buffers[i] = i;
    }
    free_circular_buffer(cb);
}

extern char is_full_circular_buffer (circular_buffer_t* buffer);
Test (circular_buffer, is_empty)
{
    circular_buffer_t* cb = new_circular_buffer(3);

    char empty = is_empty_circular_buffer(cb);
    char full = is_full_circular_buffer(cb);

    cr_assert(0 != empty, "Is expected to be empty");
    cr_assert(0 == full, "Expected NOT to be full");
    free_circular_buffer(cb);
}

Test (circular_buffer, alloc_1)
{
    circular_buffer_t* cb = new_circular_buffer(3);

    int r = alloc_entry_in_circular_buffer (cb);
    char empty = is_empty_circular_buffer(cb);
    char full = is_full_circular_buffer(cb);

    cr_assert(0 == empty, "Expected NOT to be empty");
    cr_assert(0 == full, "Expected NOT to be full");
    cr_assert(0 <= r, "Buffer number should be positive");
    free_circular_buffer(cb);
}

Test (circular_buffer, alloc_)
{
    circular_buffer_t* cb = new_circular_buffer(3);

    for (int i = 0; i < 8; i++) {
        alloc_entry_in_circular_buffer (cb);
    }

    char empty = is_empty_circular_buffer(cb);
    char full = is_full_circular_buffer(cb);
    cr_assert(0 == empty, "Expected NOT to be empty");
    cr_assert(0 != full, "Expected to be full");
    free_circular_buffer(cb);
}

Test (circular_buffer, deny_alloc)
{
    circular_buffer_t* cb = new_circular_buffer(3);

    for (int i = 0; i < 8; i++) {
        alloc_entry_in_circular_buffer (cb);
    }

    int r = alloc_entry_in_circular_buffer (cb);
    char empty = is_empty_circular_buffer(cb);
    char full = is_full_circular_buffer(cb);
    cr_assert(r < 0, "Last request should have been denied, instead returned:%d", r);
    cr_assert(0 == empty, "Expected NOT to be empty");
    cr_assert(0 != full, "Expected to be full");
    free_circular_buffer(cb);
}

Test (circular_buffer, free_nothing)
{
    circular_buffer_t* cb = new_circular_buffer(3);

    int r = buffer_has_been_sent(cb);

    char empty = is_empty_circular_buffer(cb);
    char full = is_full_circular_buffer(cb);
    cr_assert(r < 0, "Freeing should not have been executed");
    cr_assert(0 != empty, "Expected to be empty");
    cr_assert(0 == full, "Expected NOT to be full");
    cr_assert(0 == cb->last_sent, "Last_send should not have been updated");
    cr_assert(0 == cb->next_to_be_received, "next_received should not have been updated");

    free_circular_buffer(cb);
}

Test (circular_buffer, alloc_free_all)
{
    circular_buffer_t* cb = new_circular_buffer(3);

    alloc_entry_in_circular_buffer (cb);
    int r = buffer_has_been_sent(cb);

    char empty = is_empty_circular_buffer(cb);
    char full = is_full_circular_buffer(cb);
    cr_assert(r >= 0, "Freeing should have been executed");
    cr_assert(0 != empty, "Expected to be empty");
    cr_assert(0 == full, "Expected NOT to be full");
    cr_assert(1 == cb->last_sent, "Last_send should have been updated, not:%d", cb->last_sent);
    cr_assert(1 == cb->next_to_be_received, "next_received should have been updated, not:%d", cb->next_to_be_received);

    free_circular_buffer(cb);
}

Test (circular_buffer, alloc_4_free_all)
{
    circular_buffer_t* cb = new_circular_buffer(3);

    alloc_entry_in_circular_buffer (cb);
    alloc_entry_in_circular_buffer (cb);
    alloc_entry_in_circular_buffer (cb);
    alloc_entry_in_circular_buffer (cb);
    buffer_has_been_sent(cb);
    buffer_has_been_sent(cb);
    buffer_has_been_sent(cb);
    int r = buffer_has_been_sent(cb);

    char empty = is_empty_circular_buffer(cb);
    char full = is_full_circular_buffer(cb);
    cr_assert(r >= 0, "Freeing should have been executed");
    cr_assert(0 != empty, "Expected to be empty");
    cr_assert(0 == full, "Expected NOT to be full");
    cr_assert(4 == cb->last_sent, "Last_send should have been updated, not:%d", cb->last_sent);
    cr_assert(4 == cb->next_to_be_received, "next_received should have been updated, not:%d", cb->next_to_be_received);

    free_circular_buffer(cb);
}

Test (circular_buffer, set_data_size)
{
    circular_buffer_t* cb = new_circular_buffer(3);
    alloc_entry_in_circular_buffer (cb);
    alloc_entry_in_circular_buffer (cb);
    alloc_entry_in_circular_buffer (cb);
    alloc_entry_in_circular_buffer (cb);

    set_data_size_for_last_received_buffer(cb, 333);

    cr_assert(333 == cb->data_sizes[3], "Expected to update the 4th buffer");

    free_circular_buffer(cb);
}

Test (circular_buffer, set_data_size_on_boundary)
{
    circular_buffer_t* cb = new_circular_buffer(3);
    for (int i = 0; i < 8; i++) {
        alloc_entry_in_circular_buffer (cb);
    }
    set_data_size_for_last_received_buffer(cb, 333);

    char full = is_full_circular_buffer(cb);

    cr_assert(0 != full, "Buffer should be full");
    cr_assert(333 == cb->data_sizes[6], "Expected to update the 6th buffer");
    cr_assert(0 == cb->last_sent, "Nothing has been sent yet");
    cr_assert(7 == cb->next_to_be_received, "End of buffers");

    free_circular_buffer(cb);
}

Test (circular_buffer, sent_size)
{
    circular_buffer_t* cb = new_circular_buffer(3);
    alloc_entry_in_circular_buffer (cb);
    set_data_size_for_last_received_buffer(cb, 111);
    alloc_entry_in_circular_buffer (cb);
    set_data_size_for_last_received_buffer(cb, 222);
    alloc_entry_in_circular_buffer (cb);
    set_data_size_for_last_received_buffer(cb, 333);

    int s = get_to_be_sent_size(cb);

    cr_assert(111 == s, "Nothing has been sent yet. Got:%d", s);

    free_circular_buffer(cb);
}

/*! Whe always send the last buffer in the list. After this, the next one is the one that will be send next.
 *
 */
extern int mock_called_free_buffer;
Test (circular_buffer, one_sent)
{
    circular_buffer_t* cb = new_circular_buffer(3);
    alloc_entry_in_circular_buffer (cb);
    set_data_size_for_last_received_buffer(cb, 111);
    alloc_entry_in_circular_buffer (cb);
    set_data_size_for_last_received_buffer(cb, 222);
    alloc_entry_in_circular_buffer (cb);
    set_data_size_for_last_received_buffer(cb, 333);
    mock_called_free_buffer = 0;

    buffer_has_been_sent(cb);
    int s = get_to_be_sent_size(cb);

    cr_assert(222 == s, "Nothing has been sent yet. Got:%d", s);
    cr_assert(1 == mock_called_free_buffer, "One shared buffer should have been freed:%d", mock_called_free_buffer);
    free_circular_buffer(cb);
}

extern int mock_result_alloc_buffer;
Test(circular_buffer, out_of_shared_buffers)
{
    init_mock_shared_buffers_variables ();
    mock_result_alloc_buffer = -4;
    circular_buffer_t* cb = new_circular_buffer(3);

    int r = alloc_entry_in_circular_buffer (cb);

    cr_assert(-4 == r, "function should return 'alloc_buffer' result when no shared buffer is available, not:%d", r);
}