#include <criterion/criterion.h>
#include "../src/shared_buffers.h"

Test(buffers, init)
{
	int i = available_buffers();
	cr_expect(i == 0, "Initial number of buffer should be 0, was:%d", i);
	init_shared_buffers();
	cr_assert(available_buffers() == MAX_BUFFERS, "Init() method should have allocated %d buffers", MAX_BUFFERS);
}

Test(buffers, allocate2)
{
	cr_expect(available_buffers() == 0, "Initial number of buffer should be -1, was:%d", available_buffers());
	init_shared_buffers();
	cr_assert(alloc_buffer() == MAX_BUFFERS - 1, "alloc_buffer() after init() should return last buffer in the list:%d", MAX_BUFFERS-1);
	cr_assert(alloc_buffer() == MAX_BUFFERS - 2, "alloc_buffer() after init() should return last buffer in the list:%d", MAX_BUFFERS-2);
}

Test(buffers, free1)
{
	init_shared_buffers();
	int i = alloc_buffer();
	alloc_buffer();
	free_buffer(i);
	cr_assert(available_buffers() == MAX_BUFFERS - 1, "free a buffer is expected to increase the number of available buffers");
}

Test(buffers, free_middle)
{
	init_shared_buffers();
	int i1 = alloc_buffer();
	int i2 = alloc_buffer();
	int i3 = alloc_buffer();
	int i4 = alloc_buffer();
	cr_assert(alloc_buffer() == MAX_BUFFERS-5, "alloc_buffer() after init() should return last buffer in the list:%d", MAX_BUFFERS-5);
	
	free_buffer(i3);

	cr_assert(available_buffers() == MAX_BUFFERS - 4, "free a buffer is expected to increase the number of available buffers");
}

Test(buffers, override_NUmber_buffers)
{
	override_max_buffers(1);
	init_shared_buffers();
	cr_assert(available_buffers() == 1, "Expected to be 1");
	int i = alloc_buffer();
	cr_assert(available_buffers() == 0, "Expected to be 0");
}

Test(buffers, allocate_everything)
{
	override_max_buffers(3);
	init_shared_buffers();
	int i1 = alloc_buffer();
	int i2 = alloc_buffer();
	int i3 = alloc_buffer();
	int i4 = alloc_buffer();
	cr_assert(i4==-1, "Expected -1, received:%d", i4);
	cr_assert(available_buffers() == 0, "Expected to be empty");
}

Test(buffers, free_list)
{
	override_max_buffers(3);
	init_shared_buffers();
	int i1 = alloc_buffer();
	int i2 = alloc_buffer();
	int i3 = alloc_buffer();
	int i4 = alloc_buffer();
	free_buffer(i3);
	free_buffer(i1);
	i1 = alloc_buffer();
	cr_assert(i1==2, "Expected last freed instead of %d", i1);
	i3 = alloc_buffer();
	cr_assert(i3==0, "Expected last freed instead of %d", i3);
	cr_assert(available_buffers() == 0, "Expected to be empty");
}
