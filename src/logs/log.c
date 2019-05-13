#include "log.h"
#include <stdio.h>
#include <stdlib.h>

void log_msg(char msg[])
{
	printf("%s\n", msg);
}

void log_fatal(char *fmt, ...)
{
	va_list argptr;
	vprintf(fmt, argptr);
	exit(0);
}
