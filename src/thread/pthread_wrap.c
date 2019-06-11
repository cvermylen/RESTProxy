#include "pthread_wrap.h"
int pthread_create_wrapper (pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg)
{
    return pthread_create(thread, attr, start_routine, arg);
}

int pthread_join_wrapper (pthread_t thread, void** p)
{
    return pthread_join (thread, p);
}