#ifndef FILE_CONNECTOR_H
#define FILE_CONNECTOR_H

#include <stdio.h>

typedef struct {
    char *filename;
    FILE *file;
    void (*output_callback)(void *parms);
} ri_file_connector_t;

void open_file (ri_file_connector_t *conn);

void close_file (ri_file_connector_t *conn);

int file_reader(ri_file_connector_t *conn, char* dest, int max_buffer_size);

void file_writer(void* params);

void release_runtime_file_connector(ri_file_connector_t *conn);

#endif
