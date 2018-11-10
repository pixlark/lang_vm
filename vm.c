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

void init_object(Object * object, enum Object type)
{
	object->type = type;
	switch (type) {
	case OBJ_STRING:
		object->string = (String) { .len = 0, .string = NULL };
		break;
	}
}

Object * allocate_object(VM * vm, enum Object type)
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
	Object * object = &(memory[index].object);
	init_object(object, type);
	return object;
}

bool vm_step(VM * vm)
{
	Instr instr = vm->source[vm->prog_counter++];
	switch (instr.instr) {
	case INSTR_HALT:
		return false;
	case INSTR_ALLOCG: {
		char * symbol = instr.op_0.symbol;
		enum Object type = instr.op_1.obj_type;
		Object * object = allocate_object(vm, type);
		Value value = (Value) { VAL_OBJECT, .obj = object };
		table_add_symbol(vm->global_table, symbol, value);
	} break;
	case INSTR_LOAD: {
		char * symbol = instr.op_0.symbol;
		if (!table_symbol_exists(vm->global_table, symbol)) {
			internal_error("Tried to LOAD a symbol that doesn't exist");
		}
		Value value = table_get_symbol(vm->global_table, symbol);
		sb_push(vm->stack, value);
	} break;
	case INSTR_PUSHINT: {
		Value value = (Value) { VAL_INT, ._int = instr.op_0.im_int };
		sb_push(vm->stack, value);
	} break;
	}
	return true;
}

Instr i_allocg(char * symbol, enum Object type)
{
	Instr instr;
	instr.instr = INSTR_ALLOCG;
	instr.op_0 = (union Operand) { .symbol = symbol };
	instr.op_1 = (union Operand) { .obj_type = type };
	return instr;
}

Instr i_load(char * symbol)
{
	Instr instr;
	instr.instr = INSTR_LOAD;
	instr.op_0 = (union Operand) { .symbol = symbol };
	return instr;
}

Instr i_pushint(int64_t im_int)
{
	Instr instr;
	instr.instr = INSTR_PUSHINT;
	instr.op_0 = (union Operand) { .im_int = im_int };
	return instr;
}

void print_debug_memory(VM * vm)
{
	Mem_Piece * memory = ((Mem_Piece*) vm->memory);
	bool anything_printed = false;
	for (int i = 0; i < vm->memory_capacity; i++) {
		if (memory[i].allocated) {
			anything_printed = true;
			Object * object = &(memory[i].object);
			printf("[@%p, %u] %s : ",
				object, i, obj_type_str(object->type));
			print_object(object);
		}
	}
	if (!anything_printed) {
		printf("Nothing allocated\n");
	}
}

void print_debug_stack(VM * vm)
{
	for (int i = sb_count(vm->stack) - 1; i >= 0; i--) {
		print_value(vm->stack[i]);
	}
}

int main()
{
	Instr test_source[] = {
		i_allocg("test", OBJ_STRING),
		i_load("test"),
		i_pushint(15),
		(Instr) { INSTR_HALT },
	};
	VM * vm = create_vm(test_source, 128);
	do {
		printf("Stack: \n");
		print_debug_stack(vm);
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
