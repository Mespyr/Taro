#include "compiler.h"

void Compiler::handle_mov_reg_optim() {
	Instruction i = fn_key.second.at(idx);

	Argument write_reg = i.arguments.at(0);
	if (write_reg.read_pointer || write_reg.type != ARG_REGISTER) return;
	Argument save_value = i.arguments.at(1);

	switch (save_value.type) {

	case ARG_INTEGER: {
		if (stored_registers.count(write_reg.reg_value)) {
			int64_t diff = save_value.int_value - stored_registers.at(write_reg.reg_value);
			if (diff == 0)
				fn_key.second.erase(fn_key.second.begin() + idx);
			else if (diff == -1) {
				i.arguments.pop_back();
				i.type = INSTRUCTION_DEC;
				fn_key.second.at(idx) = i;
			}
			else if (diff == 1) {
				i.arguments.pop_back();
				i.type = INSTRUCTION_INC;
				fn_key.second.at(idx) = i;
			}
			else if (diff < 0) {
				i.arguments.pop_back();
				i.arguments.push_back(Argument(std::abs(diff)));
				i.type = INSTRUCTION_SUB;
				fn_key.second.at(idx) = i;
			}
			else if (diff > 0) {
				i.arguments.pop_back();
				i.arguments.push_back(Argument(diff));
				i.type = INSTRUCTION_ADD;
				fn_key.second.at(idx) = i;
			}
			stored_registers[write_reg.reg_value] = save_value.int_value;
			break;
		}
		else stored_registers.insert({write_reg.reg_value, save_value.int_value});

		if (save_value.int_value == 0) {
			i.arguments.pop_back();
			i.arguments.push_back(i.arguments.at(0));
			i.type = INSTRUCTION_XOR;
			fn_key.second.at(idx) = i;
		}
	} break;

	case ARG_REGISTER:
	case ARG_VARIABLE:
		remove_stored_register(write_reg.reg_value);
		break;
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
		case INSTRUCTION_LABEL:
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
