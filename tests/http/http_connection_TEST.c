#include "../../src/http/http_connection.h"
#include "../../src/route.h"
#include <criterion/criterion.h>
#include <stack.h>

Test (new_http_connection, result_should_never_be_null)
{
    ri_connection_t* c = new_http_connection(NULL);

    cr_assert_not_null (c, "Should not be null");
    cr_assert_not_null (c->requestReplies, "request_replies stsack should not be null");
}

Test (new_http_connection, request_replies_should_be_stack)
{
    ri_connection_t* c = new_http_connection(NULL);
    stack_head_t* st = c->requestReplies;
    cr_assert (0 == stack_depth(st), "Depth should be 0 after intialization, not:%d", stack_depth(st));
    stack_push (st, (void*)0xFFFFFFFFl);
    cr_assert(1 == stack_depth(st), "Depth should be 1 after 1 push, not:%d", stack_depth(st));
    cr_assert (0xFFFFFFFFl == (long)stack_pop(st), "Should retrieve the value with a pop");
}
