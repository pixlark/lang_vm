#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "error.h"
#include "stretchy_buffer.h"
#include "table.h"
#include "types.h"

typedef struct {
	bool allocated;
	Object object;
} Mem_Piece;

VM * create_vm(Instr * source, size_t memory_capacity)
{
	VM * vm = (VM*) malloc(sizeof(VM));
	vm->source = source;
	vm->prog_counter = 0;
	vm->stack = NULL;
	vm->memory_capacity = memory_capacity;
	vm->memory = (Mem_Piece*) calloc(memory_capacity, sizeof(Mem_Piece));
	vm->global_table = table_create(128);
}

Object * allocate_object(VM * vm)
{
	size_t index = 0;
	while (!((Mem_Piece*) vm->memory)[index].allocated) {
		index++;
		if (index >= vm->memory_capacity) {
			internal_error("Ran out of memory to allocate objects");
		}
	}
	return &(((Mem_Piece*) vm->memory)[index].object);
}

bool vm_step(VM * vm)
{
	Instr instr = vm->source[vm->prog_counter++];
	switch (instr.instr) {
	case INSTR_ALLOCG: {
		char * symbol = instr.op_0.symbol;
		enum Object type = instr.op_1.obj_type;
		Object * object = allocate_object(vm);
		object->type = type;
		table_add_symbol(vm->global_table, symbol, object);
	} break;
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
