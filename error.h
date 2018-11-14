#pragma once

#include <stdarg.h>

#include "types.h"

void internal_error(const char * fmt, ...);
void typeassert(enum Value got, enum Value expected);
void eassert(bool condition, const char * fmt, ...);
