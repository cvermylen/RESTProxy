#ifndef FILE_CONNECTOR_H
#define FILE_CONNECTOR_H

#include <stdio.h>

typedef struct {
    char *filename;
    FILE *file;
    void (*output_callback)(void *parms);
} ri_file_connector_t;

int read_from_file(int fd, char* buffer, int max_size);

void file_writer(void* params);

void release_runtime_file_connector(ri_file_connector_t *conn);

#endif
