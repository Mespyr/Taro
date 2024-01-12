#include "compiler.h"

void Compiler::handle_write_data_to_ptr(Op op) {
	// rax is the ptr to write to
	// rbx is the data
	switch (op.mode) {
	case MODE_8BIT:
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX, true), Argument(REGISTER_BL)));
		break;
	case MODE_64BIT:
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX, true), Argument(REGISTER_RBX)));
		break;
	case MODE_STRUCT: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(REGISTER_RBX, true)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX, true), Argument(REGISTER_RCX)));
		for (int i = 8; i < op.int_operand_2; i+=8) {
			asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(REGISTER_RBX, true, INFER_NONE, i)));
			asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX, true, INFER_NONE, i), Argument(REGISTER_RCX)));
		}
	} break;
	};
}

void Compiler::handle_copy_ptr_to_ptr(Op op) {
	// rax is ptr to write to
	// rbx is ptr to read from
	switch (op.mode) {
	case MODE_8BIT:
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_CL), Argument(REGISTER_RBX, true)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX, true), Argument(REGISTER_CL)));
		break;
	case MODE_64BIT:
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(REGISTER_RBX, true)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX, true), Argument(REGISTER_RCX)));
		break;
	case MODE_STRUCT:
		print_error_at_loc(op.loc, "compiler error! this opcode (" + std::to_string(op.type) + ") should have MODE_STRUCT passed into it. This is probably a bug in the parsing or type-checking stage");
		exit(1);
	}
}

void Compiler::handle_read_data_from_ptr(Op op) {
	// rax is the pointer to read from
	// rbx is the data pushed onto the stack
	// unless pushing struct; we then push rax onto stack
	switch (op.mode) {
	case MODE_8BIT:
		asmp.new_inst(asmp.inst_xor(Argument(REGISTER_RBX), Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_BL), Argument(REGISTER_RAX, true)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
		break;
	case MODE_64BIT:
		asmp.new_inst(asmp.inst_xor(Argument(REGISTER_RBX), Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RBX), Argument(REGISTER_RAX, true)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
		break;
	case MODE_STRUCT:
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
		break;
	}
}
