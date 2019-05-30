#include <criterion/criterion.h>
#include "../../src/buffers/circular_buffer.h"
#include <math.h>
#include <signal.h>

#include <stdio.h>

int mock_return_read_data;
char* mock_transfer_read_data_buffer;
int read_data(int fd, char* buffer, int buffer_size)
{
    strcpy(buffer, mock_transfer_read_data_buffer);
    return mock_return_read_data;
}

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

extern unsigned int circular_increment (unsigned int value, unsigned int mask);
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
    circular_buffer_t* cb = new_circular_buffer(3, 7, read_data, 5);

    cr_assert_not_null(cb, "new method expected to return something");
    cr_assert(3 == cb->size, "The 'size' property should be initialized at 3, not:%d", cb->size);
    cr_assert(7 == cb->fd, "'fd' should have been set up");
    cr_assert(&read_data == cb->feeder, "Address of feed function should have been set up");
    cr_assert(5 == cb->max_size, "max_size field should have been set up");
    free_circular_buffer(cb);
}


Test (circular_buffer, buffer_access_ok)
{
    circular_buffer_t* cb = new_circular_buffer(3, 7, NULL, 5);

    for (int i = 0; i < 8; i++) {
        cb->buffers[i] = i;
    }
    free_circular_buffer(cb);
}

extern char is_full_circular_buffer (circular_buffer_t* buffer);
Test (circular_buffer, is_empty)
{
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);

    char empty = is_empty_circular_buffer(cb);
    char full = is_full_circular_buffer(cb);

    cr_assert(0 != empty, "Is expected to be empty");
    cr_assert(0 == full, "Expected NOT to be full");
    free_circular_buffer(cb);
}

Test (circular_buffer, alloc_1)
{
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);

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
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);

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
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);

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
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);

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
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);

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
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);

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
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);
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
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);
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
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);
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
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);
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
    circular_buffer_t* cb = new_circular_buffer(3, 0, NULL, 0);

    int r = alloc_entry_in_circular_buffer (cb);

    cr_assert(-4 == r, "request_type should return 'alloc_buffer' result when no shared buffer is available, not:%d", r);
}

Test(circular_ptr, copy)
{
    circular_ptr_t from;
    circular_ptr_t to;
    to.buff_pos = -1;
    to.circ_index = -1;
    from.buff_pos = 3;
    from.circ_index = 5;

    copy_circ_pointers(&to, &from);

    cr_assert(3 == to.buff_pos, "'buff_pos' should be 3, not:%d", to.buff_pos);
    cr_assert(5 == to.circ_index, "'circ_index' should be 5, not:%d", to.circ_index);
}

Test(op_distance_circ_pointers, zero)
{
    circular_ptr_t from;
    from.circ_index = 0;
    from.buff_pos = 2;
    circular_ptr_t to;
    to.circ_index = 0;
    to.buff_pos = 2;

    int res = op_distance_circ_pointers(NULL, &from, &to);

    cr_assert(0 == res, "Supposed to be the same, not:%d", res);
}

Test(circular_ptr, distance_one_buffer)
/*! In this case, we don't need an actual buffer.
 *
 */
{
    circular_ptr_t from;
    from.circ_index = 0;
    from.buff_pos = 2;
    circular_ptr_t to;
    to.circ_index = 0;
    to.buff_pos = 5;

    int distance = op_distance_circ_pointers(NULL, &from, &to);

    cr_assert(3 == distance, "Distance expected to be 3, not:%d", distance);
}

Test(circular_ptr, distance_two_buffers)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    for (int i = 0; i < 8; i++) {
        cb->data_sizes[i] = -1;
    }
    cb->data_sizes[7] = 10;
    //cb->data_sizes[0] = doesn't matter, even if the 'to' points to it
    circular_ptr_t from;
    from.circ_index = 7;
    from.buff_pos = 0;

    circular_ptr_t to;
    to.circ_index = 0;
    to.buff_pos = 5;

    int distance = op_distance_circ_pointers(cb, &from, &to);

    cr_assert(15 == distance, "Distance expected to be 10, not:%d", distance);
}

Test(circular_ptr, distance_five_buffers)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    for (int i = 0; i < 8; i++) {
        cb->data_sizes[i] = -1;
    }
    cb->data_sizes[7] = 10;
    cb->data_sizes[0] = 3;
    cb->data_sizes[1] = 21;
    cb->data_sizes[2] = 0;
    //cb->data_sizes[3] = doesn't matter, even if the 'to' points to it
    circular_ptr_t from;
    from.circ_index = 7;
    from.buff_pos = 5;

    circular_ptr_t to;
    to.circ_index = 3;
    to.buff_pos = 5;

    int distance = op_distance_circ_pointers(cb, &from, &to);

    cr_assert(5+3+21+0+5 == distance, "Distance expected to be 34, not:%d", distance);
}

int mock_called_feeder;
int mock_result_feeder;
int mock_feeder(int fd, char* buffer, int size)
{
    mock_called_feeder += 1;
    return mock_result_feeder;
}

Test(circular_ptr, add_inside_one_buffer_no_exceed_limit)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes[0] = 10;
    cb->feeder = mock_feeder;
    mock_called_feeder = 0;

    circular_ptr_t from;
    from.circ_index = 0;
    from.buff_pos = 5;

    circular_ptr_t* result = op_add_circ_pointers(cb, &from, 3);

    cr_assert(0 == from.circ_index, "'circ_index' not supposed to have changed in 'from':%d", from.circ_index);
    cr_assert(from.circ_index == result->circ_index, "'circ_index' not supposed to be different:%d", from.circ_index);
    cr_assert(8 == from.buff_pos, "'buff_pos' not as expected (8) instead of:%d", result->buff_pos);
    cr_assert(from.buff_pos == result->buff_pos, "'buff_pos' not supposed to be different:%d", result->buff_pos);
    cr_assert(0 == mock_called_feeder, "'feeder' not supposed to be called, %d", mock_called_feeder);
}

Test(circular_ptr, add_overlap_one_buffer_no_load)
{
    circular_buffer_t* cb = new_circular_buffer(3, 0, mock_feeder, 1024);
    cb->data_sizes[0] = 10;
    cb->data_sizes[1] = 20;
    cb->next_to_be_received = 2;
    mock_called_feeder = 0;
    mock_result_feeder = 5;
    mock_result_alloc_buffer = 10;

    circular_ptr_t from;
    from.circ_index = 0;
    from.buff_pos = 5;

    circular_ptr_t* result = op_add_circ_pointers(cb, &from, 10);

    cr_assert(1 == from.circ_index, "'circ_index' expected to be 1, not:%d", from.circ_index);
    cr_assert(from.circ_index == result->circ_index, "'circ_index' not supposed to be different:%d", from.circ_index);
    cr_assert(5 == from.buff_pos, "'buff_pos' not as expected (5) instead of:%d", result->buff_pos);
    cr_assert(from.buff_pos == result->buff_pos, "'buff_pos' not supposed to be different:%d", result->buff_pos);
    cr_assert(0 == mock_called_feeder, "'feeder' not supposed to be called, %d", mock_called_feeder);
}

Test(circular_ptr, add_overlap_one_buffer_load)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->buffers = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes[0] = 6;
    cb->next_to_be_received = 1;
    cb->feeder = mock_feeder;
    mock_result_feeder = 5;
    mock_called_feeder = 0;

    mock_result_alloc_buffer = 2;

    circular_ptr_t from;
    from.circ_index = 0;
    from.buff_pos = 5;

    circular_ptr_t* result = op_add_circ_pointers(cb, &from, 3);

    cr_assert(1 == from.circ_index, "'circ_index' not supposed to have changed in 'from':%d", from.circ_index);
    cr_assert(from.circ_index == result->circ_index, "'circ_index' not supposed to be different:%d", from.circ_index);
    cr_assert(2 == from.buff_pos, "'buff_pos' not as expected (8) instead of:%d", result->buff_pos);
    cr_assert(from.buff_pos == result->buff_pos, "'buff_pos' not supposed to be different:%d", result->buff_pos);
    cr_assert(1 == mock_called_feeder, "'feeder' supposed to be called once, %d", mock_called_feeder);
}

Test(circular_ptr, add_overlap_3_buffers_load) {
    circular_buffer_t *cb = (circular_buffer_t *) malloc(sizeof(circular_buffer_t));
    cb->size = 3;
    cb->data_sizes = (int *) malloc(sizeof(int) * 8);
    cb->buffers = (int *) malloc(sizeof(int) * 8);
    cb->data_sizes[0] = 6;
    cb->next_to_be_received = 1;
    cb->feeder = mock_feeder;
    mock_result_feeder = 5;
    mock_called_feeder = 0;

    mock_result_alloc_buffer = 2;

    circular_ptr_t from;
    from.circ_index = 0;
    from.buff_pos = 5;

    circular_ptr_t *result = op_add_circ_pointers(cb, &from, 18);
    cr_assert(4 == from.circ_index, "'circ_index' not supposed to have changed in 'from':%d", from.circ_index);
    cr_assert(from.circ_index == result->circ_index, "'circ_index' not supposed to be different:%d", from.circ_index);
    cr_assert(2 == from.buff_pos, "'buff_pos' not as expected (8) instead of:%d", result->buff_pos);
    cr_assert(from.buff_pos == result->buff_pos, "'buff_pos' not supposed to be different:%d", result->buff_pos);
    cr_assert(4 == mock_called_feeder, "'feeder' supposed to be called 4*, %d", mock_called_feeder);
}

Test(op_sub_circ_pointer, ask_is_zero)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));

    circular_ptr_t from;
    from.circ_index = 0;
    from.buff_pos = 5;

    circular_ptr_t* res = op_sub_circ_pointer (cb, &from, 0);

    cr_assert(from.circ_index == res->circ_index, "'circ_index' should stay the same, was:%d", res->circ_index);
    cr_assert(from.buff_pos == res->buff_pos, "'buff_pos' should stay the same, was:%d", res->buff_pos);
    free(res);
}

Test(op_sub_circ_pointer, stay_in_same_buffer)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));

    circular_ptr_t from;
    from.circ_index = 0;
    from.buff_pos = 5;

    circular_ptr_t* res = op_sub_circ_pointer (cb, &from, 3);

    cr_assert(from.circ_index == res->circ_index, "'circ_index' should stay the same, was:%d", res->circ_index);
    cr_assert(from.buff_pos == res->buff_pos + 3, "'buff_pos' should stay the same, was:%d", res->buff_pos);
    free(res);
}

Test(op_sub_circ_pointer, back_two_buffers)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->buffers = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes[0] = 10;
    cb->data_sizes[7] = 10;
    cb->data_sizes[6] = 10;

    circular_ptr_t from;
    from.circ_index = 0;
    from.buff_pos = 5;

    circular_ptr_t* res = op_sub_circ_pointer (cb, &from, 20);

    cr_assert(6 == res->circ_index, "'circ_index' should be 6, was:%d", res->circ_index);
    cr_assert(5 == res->buff_pos, "'buff_pos' should be 5, was:%d", res->buff_pos);
    free(res);
}

extern char* mock_result_get_buffer[];
extern int mock_param1_get_buffer;
Test(get_char_ptr_from_buffer, A)
{
    mock_result_get_buffer[3] = "ABCDEF";
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->buffers = (int*) malloc (sizeof(int) * 8);
    cb->buffers[2] = 3;

    circular_ptr_t ptr;
    ptr.buff_pos = 3;
    ptr.circ_index = 2;

    char* result = get_char_ptr_from_buffer (cb, &ptr);

    cr_assert(3 == mock_param1_get_buffer, "'get_buffer' should have been called with '3', not:%d", mock_param1_get_buffer);
    cr_assert_not_null(result, "Not expected the result to be null");
    cr_assert('D' == result[0], "expected result was 'D', not:%c", result[0]);
    cr_assert('E' == result[1], "Expected the second character to be 'E', not:%c", result[1]);
}

Test(cmp_circular_ptr, equals)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;

    circular_ptr_t l;
    l.circ_index = 3;
    l.buff_pos = 5;

    circular_ptr_t r;
    r.circ_index = 3;
    r.buff_pos = 5;

    int res = cmp_circular_ptr (cb, &l, &r);

    cr_assert(0 == res, "The 2 ptrs are the same, instead returned:%d", res);
}

Test(cmp_circular_ptr, less_in_same_buffer)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;

    circular_ptr_t l;
    l.circ_index = 3;
    l.buff_pos = 2;

    circular_ptr_t r;
    r.circ_index = 3;
    r.buff_pos = 5;

    int res = cmp_circular_ptr (cb, &l, &r);

    cr_assert(0 > res, "The l < r, instead returned:%d", res);
}

Test(cmp_circular_ptr, less_in_previous_buffer)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->last_sent = 1;
    cb->next_to_be_received = 4;

    circular_ptr_t l;
    l.circ_index = 2;
    l.buff_pos = 5;

    circular_ptr_t r;
    r.circ_index = 3;
    r.buff_pos = 5;

    int res = cmp_circular_ptr (cb, &l, &r);

    cr_assert(0 > res, "l < r, instead returned:%d", res);
}

Test(cmp_circular_ptr, less_past_boundary)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->last_sent = 6;
    cb->next_to_be_received = 3;

    circular_ptr_t l;
    l.circ_index = 7;
    l.buff_pos = 5;

    circular_ptr_t r;
    r.circ_index = 1;
    r.buff_pos = 5;

    int res = cmp_circular_ptr (cb, &l, &r);

    cr_assert(0 > res, "l < r, instead returned:%d", res);
}

Test(cmp_circular_ptr, greater_in_same_buffer)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;

    circular_ptr_t l;
    l.circ_index = 3;
    l.buff_pos = 5;

    circular_ptr_t r;
    r.circ_index = 3;
    r.buff_pos = 2;

    int res = cmp_circular_ptr (cb, &l, &r);

    cr_assert(0 < res, "The l > r, instead returned:%d", res);
}

Test(cmp_circular_ptr, greater_in_next_buffer)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->last_sent = 1;
    cb->next_to_be_received = 4;

    circular_ptr_t l;
    l.circ_index = 3;
    l.buff_pos = 5;

    circular_ptr_t r;
    r.circ_index = 2;
    r.buff_pos = 5;

    int res = cmp_circular_ptr (cb, &l, &r);

    cr_assert(0 < res, "l > r, instead returned:%d", res);
}

Test(cmp_circular_ptr, greater_past_boundary)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->last_sent = 6;
    cb->next_to_be_received = 3;

    circular_ptr_t l;
    l.circ_index = 1;
    l.buff_pos = 5;

    circular_ptr_t r;
    r.circ_index = 7;
    r.buff_pos = 5;

    int res = cmp_circular_ptr (cb, &l, &r);

    cr_assert(0 < res, "l > r, instead returned:%d", res);
}

Test(cmp_ptr_to_last_received, equals)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->last_sent = 6;
    cb->next_to_be_received = 3;
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes[2] = 55;

    circular_ptr_t ptr;
    ptr.circ_index = 2;
    ptr.buff_pos = 54;

    int res = cmp_ptr_to_last_received (cb, &ptr);

    cr_assert(0 == res, "ptr should point to the last character received, result is:%d", res);
}

Test(cmp_ptr_to_last_received, less_no_rotation)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->last_sent = 6;
    cb->next_to_be_received = 3;
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes[2] = 55;

    circular_ptr_t ptr;
    ptr.circ_index = 2;
    ptr.buff_pos = 53;

    int res = cmp_ptr_to_last_received (cb, &ptr);

    cr_assert(0 > res, "ptr should point to the last character received, result is:%d", res);
}


Test(cmp_ptr_to_last_received, less_with_rotation_at_border)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->last_sent = 6;
    cb->next_to_be_received = 0; // last received will be @ 7
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes[6] = 55;

    circular_ptr_t ptr;
    ptr.circ_index = 6;
    ptr.buff_pos = 53;

    int res = cmp_ptr_to_last_received (cb, &ptr);

    cr_assert(0 > res, "ptr should point to the last character received, result is:%d", res);
}

Test(cmp_ptr_to_last_received, less_with_rotation)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->size = 3;
    cb->last_sent = 6;
    cb->next_to_be_received = 1; // last received will be @ 7
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes[0] = 55;
    cb->last_sent = 5;

    circular_ptr_t ptr;
    ptr.circ_index = 1;
    ptr.buff_pos = 53;

    int res = cmp_ptr_to_last_received (cb, &ptr);

    cr_assert(0 > res, "ptr should point to the last character received, result is:%d", res);
}

extern int mock_called_get_buffer;
Test(buffer_2_str_copy, empty)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->buffers = (int*) malloc (sizeof(int) * 8);

    char empty_buffer[] = "     ";
    mock_result_get_buffer[2] = empty_buffer;
    mock_called_get_buffer = 0;

    circular_ptr_t start;
    start.circ_index = 2;
    start.buff_pos = 3;

    circular_ptr_t end;
    end.circ_index = 2;
    end.buff_pos = 3;

    char* result = buffer_2_str_copy (cb, &start, &end);

    cr_assert(0L == strlen(result), "Empty string, not of size:%lu", strlen(result));
    cr_assert(1 == mock_called_get_buffer, "'get_buffer' should have been called 1, not:%d", mock_called_get_buffer);
    cr_assert(2 == mock_param1_get_buffer, "Should have called on buffer 2, not:%d", mock_param1_get_buffer);
}

Test(buffer_2_str_copy, same_buffer)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->buffers = (int*) malloc (sizeof(int) * 8);

    char empty_buffer[] = "01234MyString6789";
    mock_result_get_buffer[1] = empty_buffer;

    cb->buffers[2] = 1;

    circular_ptr_t start;
    start.circ_index = 2;
    start.buff_pos = 5;

    circular_ptr_t end;
    end.circ_index = 2;
    end.buff_pos = 13;

    char* result = buffer_2_str_copy (cb, &start, &end);

    cr_assert(8L == strlen(result), "Empty string, not of size:%lu", strlen(result));
    cr_assert(1 == mock_param1_get_buffer, "Should have called on buffer 1, not:%d", mock_param1_get_buffer);
    cr_assert(strcmp("MyString", result) == 0, "Expected 'MyString', not:'%s'", result);
}

Test(buffer_2_str_copy, overlap)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->buffers = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->size = 3;

    char buffer1[] = "01234My";
    mock_result_get_buffer[5] = buffer1;
    cb->buffers[2] = 5;
    cb->data_sizes[2] = strlen(buffer1);

    char buffer2[] = "String6789";
    mock_result_get_buffer[10] = buffer2;
    cb->buffers[3] = 10;
    cb->data_sizes[3] = strlen(buffer2);

    circular_ptr_t start;
    start.circ_index = 2;
    start.buff_pos = 5;

    circular_ptr_t end;
    end.circ_index = 3;
    end.buff_pos = 6;

    char* result = buffer_2_str_copy (cb, &start, &end);

    cr_assert(8L == strlen(result), "'MyString', not of size:%lu", strlen(result));
    cr_assert(strcmp("MyString", result) == 0, "Expected 'MyString', not:'%s'", result);
}

Test(buffer_2_str_copy, overlap_2_buffers)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->buffers = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->size = 3;

    char buffer1[] = "01234My";
    mock_result_get_buffer[5] = buffer1;
    cb->buffers[2] = 5;
    cb->data_sizes[2] = strlen(buffer1);

    char buffer2[] = "VeryLong";
    mock_result_get_buffer[10] = buffer2;
    cb->buffers[3] = 10;
    cb->data_sizes[3] = strlen(buffer2);

    char buffer3[] = "String6789";
    mock_result_get_buffer[6] = buffer3;
    cb->buffers[4] = 6;
    cb->data_sizes[4] = strlen(buffer3);

    circular_ptr_t start;
    start.circ_index = 2;
    start.buff_pos = 5;

    circular_ptr_t end;
    end.circ_index = 4;
    end.buff_pos = 6;

    char* result = buffer_2_str_copy (cb, &start, &end);

    cr_assert(16L == strlen(result), "'MyVeryLongString', not of size:%lu", strlen(result));
    cr_assert(strcmp("MyVeryLongString", result) == 0, "Expected 'MyVeryLongString', not:'%s'", result);
}

extern int mock_called_alloc_buffer;
Test(feed_next_buffer, shared_buffer_is_full)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->buffers = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->size = 3;
    cb->last_sent = 0;
    cb->next_to_be_received = 2;
    cb->feeder = mock_feeder;

    mock_called_alloc_buffer = 0;
    mock_result_alloc_buffer = -1;
    mock_called_feeder = 0;

    circular_ptr_t ptr;
    ptr.buff_pos = 55;
    ptr.circ_index = 2;

    int res = feed_next_buffer(cb, &ptr);

    cr_assert(55 == ptr.buff_pos, "'buff_pos' expected not to be updated:%d", ptr.buff_pos);
    cr_assert(2 == ptr.circ_index, "'circ_index' not expected to be changed:%d", ptr.circ_index);
    cr_assert(0 == mock_called_feeder, "Feeder should not have been called, but did happen:%d", mock_called_feeder);
    cr_assert(0 > res, "An error should be reported, not: %d", res);
}

Test(feed_next_buffer, circular_buffer_is_full)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->buffers = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->size = 3;
    cb->last_sent = 0;
    cb->next_to_be_received = 7;
    cb->feeder = mock_feeder;

    circular_ptr_t ptr;
    ptr.buff_pos = 55;
    ptr.circ_index = 2;
    mock_called_feeder = 0;

    int res = feed_next_buffer(cb, &ptr);

    cr_assert(55 == ptr.buff_pos, "'buff_pos' expected not to be updated:%d", ptr.buff_pos);
    cr_assert(2 == ptr.circ_index, "'circ_index' not expected to be changed:%d", ptr.circ_index);
    cr_assert(0 == mock_called_feeder, "Feeder should not have been called, but:%d times", mock_called_feeder);
    cr_assert(0 > res, "Result should indicate an issue, not:%d", res);
}

extern int mock_called_get_buffer;
Test(feed_next_buffer, happy_path)
{
    circular_buffer_t* cb = (circular_buffer_t*) malloc (sizeof(circular_buffer_t));
    cb->buffers = (int*) malloc (sizeof(int) * 8);
    cb->data_sizes = (int*) malloc (sizeof(int) * 8);
    cb->size = 3;
    cb->last_sent = 0;
    cb->next_to_be_received = 4;

    circular_ptr_t ptr;
    ptr.buff_pos = 55;
    ptr.circ_index = 3;

    mock_called_alloc_buffer = 0;
    mock_result_alloc_buffer = 76;
    mock_called_get_buffer= 0;

    mock_result_feeder = strlen("MyString");
    cb->feeder = mock_feeder;

    int res = feed_next_buffer(cb, &ptr);

    cr_assert(1 == mock_called_get_buffer, "'get_buffer' should have been called, calls:%d", mock_called_get_buffer);
    cr_assert(4 == ptr.circ_index, "new 'circ_index' expected to be 4, not:%d", ptr.circ_index);
    cr_assert(strlen("MyString") == ptr.buff_pos, "new 'buff_pos' expected to be %lu, not:%d", strlen("MyString"), ptr.buff_pos);
    cr_assert(strlen("MyString") == res, "'res' expected to be of the size of the string read, mot:%d", res);
}