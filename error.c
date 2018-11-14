#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "print_routines.h"

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

void typeassert(enum Value got, enum Value expected)
{
	if (got != expected) {
		fprintf(stderr, "Type error:\n\t");
		fprintf(stderr, "Mismatched types error. Expected type %s, got type %s.\n",
			val_type_str(expected), val_type_str(got));
		exit(1);
	}
}

void eassert(bool condition, const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	if (!condition) {
		fprintf(stderr, "EAssertion failed!\n\t");
		vfprintf(stderr, fmt, args);
		fprintf(stderr, "\n");
		va_end(args);
		exit(1);
	}
	va_end(args);
}
