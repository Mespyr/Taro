#include "compiler.h"

void Compiler::compare_stored_values_mov(Instruction i, AsmRegister write_reg, int64_t save_value) {
	if (stored_registers.count(write_reg)) {
		int64_t diff = save_value - stored_registers.at(write_reg);
		if (diff == 0)
			erase_idx(idx);
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
		stored_registers[write_reg] = save_value;
		return;
	}
	else stored_registers.insert({write_reg, save_value});

	if (save_value == 0) {
		i.arguments.pop_back();
		i.arguments.push_back(i.arguments.at(0));
		i.type = INSTRUCTION_XOR;
		fn_key.second.at(idx) = i;
	}
}

void Compiler::handle_mov_reg_optim() {
	Instruction i = fn_key.second.at(idx);

	Argument write_reg = i.arguments.at(0);
	if (write_reg.read_pointer || write_reg.type != ARG_REGISTER) return;
	Argument save_value = i.arguments.at(1);
	if (save_value.read_pointer) {
		remove_stored_register(write_reg.reg_value);
		return;
	}

	switch (save_value.type) {

	case ARG_INTEGER:
		compare_stored_values_mov(i, write_reg.reg_value, save_value.int_value);
		break;

	case ARG_REGISTER: {
		if (!stored_registers.count(save_value.reg_value)) {
			remove_stored_register(write_reg.reg_value);
			break;
		}
		int64_t save_value_int = stored_registers.at(save_value.reg_value);
		compare_stored_values_mov(i, write_reg.reg_value, save_value_int);
	} break;

	case ARG_VARIABLE:
		remove_stored_register(write_reg.reg_value);
		break;
	}
}
