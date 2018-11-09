#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "stretchy_buffer.h"

enum Object {
	OBJ_INTEGER,
};

typedef struct {
	enum Object type;
	union {
		int _int;
	};
} Object;

enum Instr {
	INSTR_HALT
};

typedef struct {
	enum Instr instr;
} Instr;

typedef struct {
	Instr * source;
	size_t prog_counter;
	Object * stack;
	Object * memory;
} VM;

VM * create_vm(Instr * source, size_t memory_capacity)
{
	VM * vm = (VM*) malloc(sizeof(VM));
	vm->source = source;
	vm->prog_counter = 0;
	vm->stack = NULL;
	vm->memory = (Object*) malloc(sizeof(Object) * memory_capacity);
}

bool vm_step(VM * vm)
{
	// Fetch instruction
	Instr instr = vm->source[vm->prog_counter++];
	switch (instr.instr) {
	case INSTR_HALT:
		return false;
	}
	return true;
}

Instr test_source[] = {
	(Instr) { INSTR_HALT },
};

int main()
{
	VM * vm = create_vm(test_source, 128);
	while (vm_step(vm));
	return 0;
}
