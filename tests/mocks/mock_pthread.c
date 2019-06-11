#include "../../src/thread/pthread_wrap.h"

typedef union
{
    char __size[1];
    long int __align;
} sem_t;

int sem_unlink_wrapped (const char* path)
{
    return 1;
}

sem_t* sem_open_wrapped (const char *__name, int __oflag, ...)
{
    return 0;
}

int sem_wait_wrapped (sem_t *__sem)
{
    return 0;
}

int sem_post_wrapped (sem_t *__sem)
{
    return 0;
}

int mock_called_pthread_create;

#ifdef __DARWIN_64_BIT_INO_T
typedef  __darwin_pthread_attr_t pthread_attr_t;
typedef __darwin_pthread_t pthread_t;
#else
typedef union pthread_attr_t pthread_attr_t;
typedef unsigned long int pthread_t;
#endif
int mock_return_pthread_create;
int pthread_create_wrapper(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
    mock_called_pthread_create++;
    return mock_return_pthread_create;
}

int mock_called_pthread_join;
int mock_return_pthread_join;
int pthread_join_wrapper (pthread_t thread, void** p)
{
    mock_called_pthread_join += 1;
    return mock_return_pthread_join;
}