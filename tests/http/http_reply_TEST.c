#include "../../src/http/http_reply.h"
#include <criterion/criterion.h>

Test (request, new)
{
    reply_t* r = new_http_reply();

    cr_assert_not_null(r, "'new_http_request' cannot return NULL value");
}