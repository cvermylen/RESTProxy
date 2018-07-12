#include "shared_buffers.h"
#include "log.h"
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>

static char **messages;
static int *free_buffer_list;
static int last = -1;
typedef sem_t semaphore;
static semaphore *mutex;
static void release_buffer(int);
static int max_buffers = MAX_BUFFERS;
static int tx_buffer_size = TX_BUFFER_SIZE;

void override_max_buffers(int m)
{
	max_buffers = m;
}

void override_tx_buffer_size(int t)
{
	tx_buffer_size = t;
}

void init_shared_buffers()
{
	mutex = (semaphore *)malloc(sizeof(semaphore));
	sem_unlink("/gateway_sem");
	mutex = sem_open("/gateway_sem", O_CREAT , 0644, 1);
	sem_wait(mutex);
	messages = (char**)malloc(sizeof(char*) * max_buffers);
	free_buffer_list = (int*)malloc(sizeof(int) * max_buffers);
	for(int i=0; i < max_buffers; i++){
		messages[i] = (char*)malloc(sizeof(char) * tx_buffer_size);
		release_buffer(i);
	}
	last = max_buffers -1;
	sem_post(mutex);
}

char *get_buffer(const int num)
{
	return messages[num];
}

int alloc_buffer()
{
	int current = -1;
	sem_wait(mutex);
	if(last < 0){
		log_msg("Request to alloc a buffer has failed, Network buffer pool is empty");
	}else{
		current = free_buffer_list[last];
		free_buffer_list[last--] = -1;
	}
	sem_post(mutex);
	return current;
}

void free_buffer(int buffer_no)
{
printf("free_buffer 1:%d\n", buffer_no);
	sem_wait(mutex);
printf("free buffer 2:%d\n", buffer_no);
	if(buffer_no >= 0 && buffer_no < max_buffers){
		release_buffer(buffer_no);
	}
	sem_post(mutex);
}

static void release_buffer(int buffer_no)
{
	memset(messages[buffer_no], '\0', tx_buffer_size);
	free_buffer_list[++last] = buffer_no;
}

int available_buffers()
{
	return last+1;
}
