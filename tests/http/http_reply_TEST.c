#include "../../src/http/http_reply.h"
#include <criterion/criterion.h>

Test (request, new)
{
    reply_t* r = create_reply( NULL, NULL, NULL, NULL, NULL);

    cr_assert_not_null(r, "'new_http_request' cannot return NULL value");
}