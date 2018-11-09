#include "print_routines.h"

#include <stdio.h>

const char * obj_type_str_a[] = {
	[OBJ_INT] = "int",
};

const char * obj_type_str(enum Object type)
{
	return obj_type_str_a[type];
}

void print_object(Object * object)
{
	switch (object->type) {
	case OBJ_INT:
		printf("i%d\n", object->_int);
		break;
	}
}

void print_instr(Instr instr)
{
	switch (instr.instr) {
	case INSTR_HALT:
		printf("halt\n");
		break;
	case INSTR_ALLOCG:
		printf("allocg %s %s\n", instr.op_0.symbol, obj_type_str(instr.op_1.obj_type));
		break;
	}
}
