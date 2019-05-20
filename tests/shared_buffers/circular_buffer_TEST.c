#include <criterion/criterion.h>
#include "../../src/frame_buffers/circular_buffer.h"
#include <math.h>
#include <signal.h>

#include <stdio.h>

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
}

Test (circular_buffer, alloc_3)
{
    circular_buffer_t* cb = new_circular_buffer(3);

    for (int i = 0; i < 8; i++) {
        alloc_entry_in_circular_buffer (cb);
    }


    printf("next:%d\n", cb->next_to_be_received);
    printf("mask:%x\n", ((unsigned int)pow(2, cb->size) -1));
    char empty = is_empty_circular_buffer(cb);
    char full = is_full_circular_buffer(cb);
    cr_assert(0 == empty, "Expected NOT to be empty");
    cr_assert(0 != full, "Expected NOT to be full");
}