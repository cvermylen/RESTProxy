#include "../../src/http/http_headers.h"

int mock_called_feeder;
int mock_result_feeder[100];
int mock_feeder(int fd, char* buffer, int size)
{
    mock_called_feeder += 1;
    return mock_result_feeder[mock_called_feeder-1];
}
extern int mock_called_alloc_buffer;
extern char* mock_result_get_buffer[];

int main(int argc, char** argv)
{
    char str[] = "HTTP 1/1 GET\nAccept: Apple\nServer: localhost\n\n<html>\n";
    circular_buffer_t* cb = new_circular_buffer(3, 0, mock_feeder, 1024);
    http_header_t *header = http_headers_init(cb);
    mock_called_alloc_buffer = 0;
    mock_called_feeder = 0;
    mock_result_feeder[0] = 53;
    mock_result_get_buffer[1] = str;

    decode_http_headers(header);
}
