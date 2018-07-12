#include "file_connector.h"
#include "route_instance.h"

void file_writer(void *params)
{
	ri_thread_data_t *con = (ri_thread_data_t*)params;
	fputs(con->buffer, con->content.file->file);
	fflush(con->content.file->file);
}
