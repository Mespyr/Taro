#include "compiler.h"

void Compiler::handle_stack_pop_optim() {
	Instruction i = fn_key.second.at(idx);
	remove_stored_register(i.arguments.front().reg_value);

	int64_t push_idx = find_idx_for_top_push();
	if (push_idx == -1)
		return;

	// assumes that we are popping to a register (we only pop to registers)
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

void Compiler::handle_stack_push_optim() {
	Instruction i = fn_key.second.at(idx);
	
	// check if pushing a register on the stack
	Argument arg = i.arguments.front();
	if (arg.type != ARG_REGISTER ||
		arg.read_pointer || arg.add_amount != 0)
		return;

	AsmRegister push_reg = arg.reg_value;

	// if there is no instructions before this one, return
	if (idx == 0)
		return;

	// assumes that we are popping to a register (we only pop to registers)
	Instruction prev_inst = fn_key.second.at(idx - 1);
	if (prev_inst.type != INSTRUCTION_POP)
		return;
	AsmRegister pop_reg = prev_inst.arguments.front().reg_value;

	if (push_reg != pop_reg)
		return;

	/*
	  erase pop and push if register value isn't ever used
	  - no point in reading the top value from the stack if we never use it
	  - not have had it happen in any of my tests so idk if it even works
	  - or if it even occurs in the generated code
	 */
	if (!register_is_used_before_being_set(push_reg)) {
		erase_idx(idx);
		erase_idx(idx);
		return;
	}
	
	prev_inst.type = INSTRUCTION_MOV;
	prev_inst.arguments.push_back(Argument(REGISTER_RSP, true));
	fn_key.second.at(idx - 1) = prev_inst;
	erase_idx(idx);
}
