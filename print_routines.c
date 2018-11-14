#include "print_routines.h"

#include <stdio.h>

const char * obj_type_str_a[] = {
	[OBJ_STRING] = "string",
};

const char * val_type_str_a[] = {
	[VAL_INT] = "integer",
	[VAL_OBJECT] = "object",
};

const char * oper_type_str_a[] = {
	[OPER_ADD] = "+",
	[OPER_SUB] = "-",
	[OPER_MUL] = "*",
	[OPER_DIV] = "/",
};

const char * obj_type_str(enum Object type)
{
	return obj_type_str_a[type];
}

const char * val_type_str(enum Value type)
{
	return val_type_str_a[type];
}

const char * oper_type_str(enum Oper oper)
{
	return oper_type_str_a[oper];
}

void print_object(Object * object)
{
	switch (object->type) {
	case OBJ_STRING:
		printf("\"%.*s\"\n", object->string.len, object->string.string);
		break;
	}
}

void print_value(Value value)
{
	switch (value.type) {
	case VAL_OBJECT:
		printf("%s - ", obj_type_str(value.obj->type));
		print_object(value.obj);
		break;
	case VAL_INT:
		printf("i%d\n", value._int);
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
		printf("allocg %s %s\n", instr.op_0.symbol, val_type_str(instr.op_1.val_type));
		break;
	case INSTR_LOAD:
		printf("load %s\n", instr.op_0.symbol);
		break;
	case INSTR_PUSHINT:
		printf("pushint %d\n", instr.op_0.im_int);
		break;
	case INSTR_OPER:
		printf("operator %s\n", oper_type_str(instr.op_0.oper));
		break;
	default:
		printf("some instruction %d (not setup in print_instr)\n", instr.instr);
		break;
	}
}
