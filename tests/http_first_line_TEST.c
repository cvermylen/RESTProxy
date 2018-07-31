#include "../src/http_first_line.h"
#include <criterion/criterion.h>

Test(http_first_line, op_code_1)
{
        char str[] = "GET /favicon.ico HTTP1/1";
        int op = http_decode_request_type(str, strlen(str));
        cr_assert(1 == op, "expected to return GET(1), not:%d", op);
}

Test(http_first_line, op_code_2)
{
        char str[] = "POST / HTTP1/1";
        int op = http_decode_request_type(str, strlen(str));
        cr_assert(2 == op, "expected to return POST(2), not:%d", op);
}

Test(http_first_line, op_code_3)
{
        char str[] = "Content-Type: text/html";
        int op = http_decode_request_type(str, strlen(str));
        cr_assert(0 == op, "expected to return 0, not:%d", op);
}

