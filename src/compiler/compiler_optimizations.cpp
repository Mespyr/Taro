#include "compiler.h"

void Compiler::optimize_current_func() {
	idx = 0;
	while (idx < fn_key.second.size()) {
		Instruction i = fn_key.second.at(idx);

		switch (i.type) {

		case INSTRUCTION_ADD:
		case INSTRUCTION_SUB: {
			if (i.arguments.at(1).type == ARG_INTEGER && i.arguments.at(1).int_value == 0)
				erase_idx(idx);
			else if (i.arguments.front().type == ARG_REGISTER && i.arguments.front().read_pointer == false)
				remove_stored_register(i.arguments.front().reg_value);
		} break;

		case INSTRUCTION_MOV:
			handle_mov_reg_optim();
			break;
		case INSTRUCTION_POP:
			handle_stack_pop_optim();
			break;
		case INSTRUCTION_PUSH:
			handle_stack_push_optim();
			break;

		case INSTRUCTION_CALL:
		case INSTRUCTION_LABEL:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JZ:
		case INSTRUCTION_JNZ:
			// cant be tracked across functions or loops
			stored_registers.clear();
			break;

		default:
			break;
		}
		idx++;
	}
	asmp.code.at(fn_key.first) = fn_key.second;
}

void Compiler::perform_optimizations() {
	for (auto key : asmp.code) {
		fn_key = key;
		stored_registers.clear();
		optimize_current_func();
	}
}
