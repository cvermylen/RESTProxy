#ifndef FILE_CONNECTOR_H
#define FILE_CONNECTOR_H

#include <stdio.h>

typedef struct {
    char *filename;
    FILE *file;
    void (*output_callback)(void *parms);
} ri_file_connector_t;

void file_writer(void* params);

void release_runtime_file_connector(ri_file_connector_t *conn);

#endif
