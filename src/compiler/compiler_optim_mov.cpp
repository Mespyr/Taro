#include "compiler.h"

bool Compiler::check_mov_reg_args(Instruction i) {
	// returns false if there is an something that
	// makes it so the outside function couldn't optimize
	Argument write_reg = i.arguments.at(0);
	if (write_reg.read_pointer || write_reg.type != ARG_REGISTER) return false;
	Argument save_value = i.arguments.at(1);
	if (save_value.read_pointer) {
		remove_stored_register(write_reg.reg_value);
		return false;
	}
	return true;
}

void Compiler::compare_stored_values_mov(Instruction i, AsmRegister write_reg, int64_t save_value) {
	if (stored_registers.count(write_reg)) {
		int64_t diff = save_value - stored_registers.at(write_reg);
		if (diff == 0)
			erase_idx(idx);
		else if (diff < 0)
			fn_key.second.at(idx) = asmp.inst_sub(i.arguments.front(), Argument(-diff));
		else if (diff > 0)
			fn_key.second.at(idx) = asmp.inst_add(i.arguments.front(), Argument(diff));
		stored_registers[write_reg] = save_value;
		return;
	}
	else stored_registers.insert({write_reg, save_value});

	if (save_value == 0)
		fn_key.second.at(idx) = asmp.inst_xor(i.arguments.front(), i.arguments.front());
}

void Compiler::handle_mov_reg_optim() {
	Instruction i = fn_key.second.at(idx);
	if (!check_mov_reg_args(i))
		return;

	Argument write_reg = i.arguments.at(0);
	Argument save_value = i.arguments.at(1);
	int64_t int_val;
	
	switch (save_value.type) {

	case ARG_INTEGER:
		int_val = save_value.int_value;
		break;

	case ARG_REGISTER: {
		if (!stored_registers.count(save_value.reg_value)) {
			remove_stored_register(write_reg.reg_value);
			return;
		}
		int_val = stored_registers.at(save_value.reg_value);
	} break;

	case ARG_VARIABLE:
		remove_stored_register(write_reg.reg_value);
		return;
	}

	if (stored_registers.count(write_reg.reg_value)) {
		int64_t diff = int_val - stored_registers.at(write_reg.reg_value);
		if (diff == 0)
			erase_idx(idx);
		stored_registers[write_reg.reg_value] = int_val;
	}
	else stored_registers.insert({write_reg.reg_value, int_val});
}
