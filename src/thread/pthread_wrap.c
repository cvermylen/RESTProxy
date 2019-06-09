#include "pthread_wrap.h"
int pthread_create_wrapped(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg)
{
    pthread_create(thread, attr, start_routine, arg);
}
