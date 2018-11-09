#include <stdio.h>
#include <stdlib.h>

#include "error.h"

void internal_error(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "Internal compiler error:\n\t");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}
