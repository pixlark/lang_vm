#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

enum Object {
	OBJ_STRING,
};

typedef struct {
	size_t len;
	const char * string;
} String;

typedef struct {
	enum Object type;
	union {
		String string;
	};
} Object;

enum Value {
	VAL_INT,
	VAL_OBJECT,
};

typedef struct {
	enum Value type;
	union {
		int64_t _int;
		Object * obj;
	};
} Value;

typedef struct {
	char ** keys;
	Value * values;
	bool * taken;
	size_t size;
} Symbol_Table;

enum Oper {
	OPER_ADD,
	OPER_SUB,
	OPER_MUL,
	OPER_DIV,
};

enum Instr {
	INSTR_HALT,
	/* No args */
	INSTR_ALLOCG,
	/* op_0: symbol
	   op_1: obj_type */
	INSTR_LOAD,
	/* op_0: symbol */
	INSTR_PUSHINT,
	/* op_0: int */
	INSTR_OPER,
	/* op_0: operation type */
};

union Operand {
	enum Oper oper;
	Value value;
	enum Value val_type;
	Object obj;
	enum Object obj_type;
	char * symbol;
	int64_t im_int;
};

typedef struct {
	enum Instr instr;
	union Operand op_0;
	union Operand op_1;
	union Operand op_2;
} Instr;

typedef struct {
	Instr * source;
	size_t prog_counter;
	Value * stack;
	size_t memory_capacity;
	void * memory;
	Symbol_Table * global_table;
} VM;
