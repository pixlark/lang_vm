#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "error.h"
#include "print_routines.h"
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
	Mem_Piece * memory = (Mem_Piece*) vm->memory;
	size_t index = 0;
	while (memory[index].allocated) {
		index++;
		if (index >= vm->memory_capacity) {
			internal_error("Ran out of memory to allocate objects");
		}
	}
	memory[index].allocated = true;
	return &(memory[index].object);
}

void print_debug_memory(VM * vm)
{
	Mem_Piece * memory = ((Mem_Piece*) vm->memory);
	bool anything_printed = false;
	for (int i = 0; i < vm->memory_capacity; i++) {
		if (memory[i].allocated) {
			anything_printed = true;
			printf("[@%p, %u] %s : ",
				&(memory[i].object),
				i,
				obj_type_str(memory[i].object.type));
			print_object(&(memory[i].object));
		}
	}
	if (!anything_printed) {
		printf("Nothing allocated\n");
	}
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

Instr allocg(char * symbol, enum Object type)
{
	Instr instr;
	instr.instr = INSTR_ALLOCG;
	instr.op_0 = (union Operand) { .symbol = symbol };
	instr.op_1 = (union Operand) { .obj_type = type };
	return instr;
}

int main()
{
	Instr test_source[] = {
		allocg("test", OBJ_INT),
		(Instr) { INSTR_HALT },
	};
	VM * vm = create_vm(test_source, 128);
	do {
		printf("Memory: \n");
		print_debug_memory(vm);
		printf("\n");
		printf("Instruction: ");
		print_instr(vm->source[vm->prog_counter]);
	} while (vm_step(vm));
	printf("Final memory state:\n");
	print_debug_memory(vm);
	return 0;
}
