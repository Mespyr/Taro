#include "compiler.h"

void Compiler::handle_stack_pop_optim() {
	Instruction i = fn_key.second.at(idx);
	remove_stored_register(i.arguments.front().reg_value);

	int64_t push_idx = find_idx_for_top_push();
	if (push_idx == -1)
		return;

	// assumes that we are popping to a register (we only pop to registers)
	Argument set_arg = i.arguments.front();
	if (register_is_used_before(set_arg.reg_value, push_idx))
		return;

	// This is assuming that the pushed argument is static (ie: string pointer, register, integer)
	Instruction push_inst = fn_key.second.at(push_idx);
	Argument pushed_arg = push_inst.arguments.front();
	if (arguments_equal(set_arg, pushed_arg)) {
		erase_idx(idx);
		erase_idx(push_idx);
		return;
	}
    fn_key.second.at(push_idx) = asmp.inst_mov(set_arg, pushed_arg);
	erase_idx(idx);

	/*
	  TODO:
	  check in stored registers to see if pushed reg has a value and use that as arg
	  using the reg might makes stuff weird with parralel execution and force the program to wait
	  for the prev inst to execute before moving when both could be executed at the same time

	  Steps:
	  1. check if instruction before push_idx is setting the pushed_arg (assumed register)
	  2. if it is, check after push_idx to see if register value is used at all before being set again
	  3. if the register is set before being used, remove push_idx-1 and set the 2nd arg of push_idx to the 2nd arg of push_idx-1

	  ex:
	    mov rax, 10
		mov rdx, rax
	  converts to:
	    mov rdx, 10
	*/
	//std::cout << "wow" << std::endl;
	//std::cout << fn_key.second.at(push_idx).to_string() << std::endl;

	// first, check if the argument written to the register IS ALSO a register
	if (pushed_arg.type != ARG_REGISTER || pushed_arg.read_pointer)
		return;
	// make sure previous instruction to just-created mov IS ALSO a mov
	Instruction prev_inst = fn_key.second.at(push_idx-1);
	if (prev_inst.type != INSTRUCTION_MOV)
		return;
	// check if it is setting a register and if they are the same
	Argument set_reg = prev_inst.arguments.front();
	if (set_reg.type != ARG_REGISTER || set_reg.read_pointer)
		return;
	if (set_reg.reg_value != pushed_arg.reg_value)
		return;

	// check if register is used after being set
	if (register_is_used_before_being_set_again(set_arg.reg_value, push_idx))
		return;


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
	if (!register_is_used_before_being_set_again(push_reg, idx)) {
		std::cout << "Holy Shit it happened" << std::endl;
		erase_idx(idx);
		erase_idx(idx);
		return;
	}
	
	fn_key.second.at(idx - 1) = asmp.inst_mov(prev_inst.arguments.front(), Argument(REGISTER_RSP, true));
	erase_idx(idx);
}
