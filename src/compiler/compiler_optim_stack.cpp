#include "compiler.h"

int64_t Compiler::find_idx_for_top_push() {
	int32_t indent = 0;
	for (uint32_t i = idx-1; i > 0; i--) {
		Instruction inst = fn_key.second.at(i);
		switch (inst.type) {

		case INSTRUCTION_POP:
			indent++;
			break;
			
		case INSTRUCTION_PUSH: {
			if (indent > 0) indent--;
			else return i;
		} break;

		case INSTRUCTION_CALL:
		case INSTRUCTION_LABEL:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JZ:
		case INSTRUCTION_JNZ:
		case INSTRUCTION_SYSCALL:
		case INSTRUCTION_RET: // JUST TO MAKE SURE IDK
			return -1;

		default:
			break;
		}
	}

	return -1;
}

bool Compiler::register_is_used(AsmRegister reg, uint32_t end_idx) {
	for (uint32_t i = idx-1; i > end_idx; i--) {
		Instruction inst = fn_key.second.at(i);
		switch (inst.type) {

		// 2 arity
		case INSTRUCTION_ADD:
		case INSTRUCTION_SUB:
		case INSTRUCTION_IMUL:
		case INSTRUCTION_MOV:
		case INSTRUCTION_CMP:
		case INSTRUCTION_CMOVE:
		case INSTRUCTION_CMOVG:
		case INSTRUCTION_CMOVL:
		case INSTRUCTION_CMOVGE:
		case INSTRUCTION_CMOVLE:
		case INSTRUCTION_CMOVNE:
		case INSTRUCTION_AND:
		case INSTRUCTION_OR: {
			Argument a = inst.arguments.front();
			Argument b = inst.arguments.back();
			if ((a.type == ARG_REGISTER && a.reg_value == reg) ||
				(b.type == ARG_REGISTER && b.reg_value == reg))
				return true;
		} break;

		// 1 arity
		case INSTRUCTION_INC:
		case INSTRUCTION_DEC:
		case INSTRUCTION_PUSH:
		case INSTRUCTION_POP: {
			Argument a = inst.arguments.front();
			if (a.type == ARG_REGISTER && a.reg_value == reg)
				return true;
		} break;

		/* only one that really modifies more than js the register specified
		   without using another op before or after it
		 */
		case INSTRUCTION_DIV: {
			Argument a = inst.arguments.front();
			if ((a.type == ARG_REGISTER && a.reg_value == reg) ||
				(reg == REGISTER_RAX) || (reg == REGISTER_RDX))
				return true;
		} break;

		default:
			break;
		}
	}
	return false;
}

void Compiler::handle_stack_optim() {
	Instruction i = fn_key.second.at(idx);
	remove_stored_register(i.arguments.front().reg_value);

	int64_t push_idx = find_idx_for_top_push();
	if (push_idx == -1)
		return;

	/*
	  This is assuming that
	  1. the instruction is pop (which it has to be)
	  2. we are popping to a register (which is the only time we use pop)
	 */
	Argument set_arg = i.arguments.front();
	if (register_is_used(set_arg.reg_value, push_idx))
		return;


	// This is assuming that the pushed argument is static (ie: string pointer, register, integer)
	Argument pushed_arg = fn_key.second.at(push_idx).arguments.front();
	if (arguments_equal(set_arg, pushed_arg)) {
		erase_idx(idx);
		erase_idx(push_idx);
		return;
	}

	Instruction push_inst = fn_key.second.at(push_idx);
	push_inst.type = INSTRUCTION_MOV;
	push_inst.arguments.clear();
	push_inst.arguments.push_back(set_arg);
	push_inst.arguments.push_back(pushed_arg);
    fn_key.second.at(push_idx) = push_inst;
	erase_idx(idx);
}
