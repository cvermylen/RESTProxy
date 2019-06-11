#ifndef PTHREAD_WRAP_H
#define PTHREAD_WRAP_H

#include <pthread.h>

int pthread_create_wrapper (pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

int pthread_join_wrapper (pthread_t thread, void** p);
#endif //PTHREAD_WRAP_H
