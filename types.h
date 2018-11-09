#pragma once

enum Object {
	OBJ_INTEGER,
};

typedef struct {
	enum Object type;
	union {
		int _int;
	};
} Object;

typedef struct {
	char ** keys;
	Object ** values;
	bool * taken;
	size_t size;
} Symbol_Table;

enum Instr {
	INSTR_HALT,
	/* No args */
	INSTR_ALLOCG,
	/* op_0: symbol
	   op_1: obj_type */
};

union Operand {
	Object * obj;
	enum Object obj_type;
	char * symbol;
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
	Object * stack;
	size_t memory_capacity;
	void * memory;
	Symbol_Table * global_table;
} VM;
