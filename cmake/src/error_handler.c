#include "error_handler.h"
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env;
static char *error_message;
static int error_code;

void init()
{
	int ret_code;

	ret_code = setjmp(env);
	switch(ret_code){
	case FATAL:
		printf("FATAL (%d) %s", error_code, error_message);
	default:
		printf("UNKNOW ERROR (%d) %s", error_code, error_message);
	}
	exit(1);
}

void fatal(char *msg, int errno)
{
	error_message = msg;
	error_code = errno;

	longjmp(env, FATAL);
}
