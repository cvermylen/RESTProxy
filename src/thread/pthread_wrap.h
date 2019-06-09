#ifndef PTHREAD_WRAP_H
#define PTHREAD_WRAP_H

#include <pthread.h>

int pthread_create_wrapped(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

#endif //PTHREAD_WRAP_H
