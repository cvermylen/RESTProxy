#include "../src/http_message.h"
#include <str_stack.h>
#include <criterion/criterion.h>

Test(http_message, init)
{
	http_message_t* msg = NULL;
	msg = http_message_init();
	cr_assert(msg != NULL, "init expected to return a non-NULL value");
	http_message_free(msg);
}

Test(http_message, get_empty)
{
	http_message_t* msg = http_message_init();
	stack_head_t* empty = http_message_get_header(msg, 3);
	cr_assert(is_stack_empty(empty), "expected to return NULL on prop not found");
	http_message_free(msg);
}

Test(http_message, get_1)
{
	http_message_t* msg = http_message_init();
	http_message_add_header(msg, 3, "value1");
	stack_head_t* st = http_message_get_header(msg, 3);
	cr_assert(1 == stack_depth(st), "expected stack depth to be 1, not: %d", stack_depth(st));
	char* s = str_stack_pop(st);
	cr_assert(strcmp("value1", s) == 0, "expected 'value1', not:%s", s);
	cr_assert(is_stack_empty(st), "expected stack to be empty after poping value");
	free(s);
	str_stack_free(st);
	http_message_free(msg);
}

Test(http_message, get_2)
{
	http_message_t* msg = http_message_init();
	http_message_add_header(msg, 3, "value1");
	http_message_add_header(msg, 3, "value2");
	stack_head_t* st = http_message_get_header(msg, 3);
	cr_assert(2 == stack_depth(st), "expected stack depth to be 2, not: %d", stack_depth(st));
	str_stack_free(st);
	http_message_free(msg);
}
