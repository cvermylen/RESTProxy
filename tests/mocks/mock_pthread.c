#include <semaphore.h>
int sem_unlink(const char* path)
{
    return 1;
}

sem_t* sem_open(const char *__name, int __oflag, ...)
{
    return 0;
}

int sem_wait (sem_t *__sem)
{
    return 0;
}

int sem_post (sem_t *__sem)
{
    return 0;
}