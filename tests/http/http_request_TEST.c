#include "../../src/http/http_request.h"
#include <criterion/criterion.h>

Test (request, new)
{
    request_t* r = new_http_request();

    cr_assert_not_null(r, "'new_http_request' cannot return NULL value");
}