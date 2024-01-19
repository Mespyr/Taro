#include "compiler.h"

void Compiler::handle_mov_reg_optim() {
	Instruction i = fn_key.second.at(idx);
	std::cout << idx << " " + i.to_string() << std::endl << std::endl;

	// TODO: Fix optimization
	if (i.arguments.front().type == ARG_REGISTER && i.arguments.front().read_pointer == false) {
		AsmRegister save_reg = i.arguments.front().reg_value;
		Argument reg_value = i.arguments.at(1);
		if (stored_registers.count(save_reg)) {
			if (arguments_equal(stored_registers.at(save_reg), reg_value)) {
				fn_key.second.erase(fn_key.second.begin() + idx);
				return;
			}
		}
		stored_registers.emplace(save_reg, reg_value);
	}

	if (i.arguments.at(1).type == ARG_INTEGER && i.arguments.at(1).int_value == 0) {
		i.arguments.pop_back();
		i.arguments.push_back(i.arguments.at(0));
		i.type = INSTRUCTION_XOR;
		fn_key.second.at(idx) = i;
	}
}

void Compiler::optimize_current_func() {
	idx = 0;
	for (Instruction i : fn_key.second) {
		switch (i.type) {

		case INSTRUCTION_ADD:
		case INSTRUCTION_SUB: {
			if (i.arguments.at(1).type == ARG_INTEGER && i.arguments.at(1).int_value == 0)
				fn_key.second.erase(fn_key.second.begin() + idx);
			else if (i.arguments.front().type == ARG_REGISTER && i.arguments.at(0).read_pointer == false)
				remove_stored_register(i.arguments.at(0).reg_value);
		} break;

		case INSTRUCTION_MOV:
			handle_mov_reg_optim();
			break;

		case INSTRUCTION_POP:
			remove_stored_register(i.arguments.front().reg_value);
			break;

		case INSTRUCTION_CALL:
			// cant be tracked across functions so we must clear
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
