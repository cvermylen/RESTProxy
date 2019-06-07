#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "file_connector.h"

//char resp[] = "HTTP/1.1 200 OK\nDate: Mon, 23 May 2005 22:38:34 GMT\nContent-Type: text/html; charset=UTF-8\nContent-Length: 138\nLast-Modified: Wed, 08 Jan 2003 23:11:55 GMT\nServer: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\nETag: \"3f80f-1b6-3e1cb03b\"\nAccept-Ranges: bytes\n\n<html>\n<head>\n <title>An Example Page</title>\n</head>\n<body>\n Hello World, this is a very simple HTML document.\n</body>\n</html>";
//char resp[] = "HTTP/1.1 200 OK\nDate: Mon, 23 May 2005 22:38:34 GMT\nContent-Type: text/html; charset=UTF-8\nContent-Length: 14\nLast-Modified: Wed, 08 Jan 2003 23:11:55 GMT\nServer: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\nETag: \"3f80f-1b6-3e1cb03b\"\nAccept-Ranges: bytes\n\n<html>\n</html>";

int open_file (ri_file_connector_t *conn)
{
    conn->file = fopen(conn->filename, "w");
    return 0;
}

int close_file (ri_file_connector_t *conn)
{
    fclose (conn->file);
    return 0;
}

int file_reader(ri_file_connector_t *conn, char* dest, int max_buffer_size)
{
    return fread(dest, 1, max_buffer_size, conn->file);
}

int file_writer(ri_file_connector_t *conn, char* dest, int buffer_size)
{
    fwrite (dest, 1, buffer_size, conn->file);
    fflush(conn->file);
    return 0;
}
