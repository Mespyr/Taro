#include "compiler.h"

int64_t Compiler::find_idx_for_top_push() {
	for (uint32_t i = idx-1; i > 0; i--) {
		Instruction inst = fn_key.second.at(i);
		switch (inst.type) {

		case INSTRUCTION_PUSH:
			return i;

		case INSTRUCTION_CALL:
		case INSTRUCTION_LABEL:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JZ:
		case INSTRUCTION_JNZ:
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

		case INSTRUCTION_ADD:
		case INSTRUCTION_SUB:
		case INSTRUCTION_IMUL:
		case INSTRUCTION_MOV: {
			Argument a = inst.arguments.front();
			Argument b = inst.arguments.back();
			if ((a.type == ARG_REGISTER && a.reg_value == reg) ||
				(b.type == ARG_REGISTER && b.reg_value == reg))
				return true;
		} break;

		case INSTRUCTION_INC:
		case INSTRUCTION_DEC: {
			Argument a = inst.arguments.front();
			if (a.type == ARG_REGISTER && a.reg_value == reg)
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
	std::cout << "PUSH FOUND at " << push_idx << " | " + i.to_string() + " | " + fn_key.first << std::endl;

	/*
	  This is assuming that
	  1. the instruction is pop (which it has to be)
	  2. we are popping to a register (which is the only time we use pop)
	 */
	AsmRegister set_register = i.arguments.front().reg_value;

	if (!register_is_used(set_register, push_idx))
		std::cout << i.to_string() + " " << push_idx << std::endl;
	
	/*
	Instruction push_inst = fn_key.second.at(push_idx);
	Argument push_inst_arg = push_inst.arguments.front();
	Argument pop_inst_arg = i.arguments.front();
	std::cout << push_idx << " | " + i.to_string() + " | " + push_inst.to_string() + " | " + fn_key.first << std::endl;

	if (arguments_equal(push_inst_arg, pop_inst_arg)) {
		erase_idx(idx);
		erase_idx(push_idx);
		return;
	}
	*/
	/*
	if (!push_inst_arg.read_pointer && push_inst_arg.type == ARG_REGISTER) {
		push_inst.type = INSTRUCTION_MOV;
		push_inst.arguments.clear();
		push_inst.arguments.push_back(pop_inst_arg);
		push_inst.arguments.push_back(push_inst_arg);
		fn_key.second.at(push_idx) = push_inst;
		erase_idx(idx);

		if (stored_registers.count(push_inst_arg.reg_value)) {
			if (stored_registers.count(pop_inst_arg.reg_value))
				stored_registers[pop_inst_arg.reg_value] = stored_registers.at(push_inst_arg.reg_value);
			else
				stored_registers.insert({pop_inst_arg.reg_value, stored_registers.at(push_inst_arg.reg_value)});
		}
		else remove_stored_register(pop_inst_arg.reg_value);
		}*/
}
