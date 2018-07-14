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

