#pragma once

#include "types.h"

const char * obj_type_str(enum Object type);
void print_object(Object * obj);
void print_instr(Instr instr);
void print_value(Value value);
