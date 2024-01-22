#include "compiler.h"
#include <cstdlib>
#include <error.h>

void Compiler::gen_op_asm(Op op, int func_addr) {
	switch (op.type) {

	case OP_DUMP: {
		asmp.compile_dump = true;
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
		asmp.new_inst(asmp.inst_call("dump"));
	} break;

	case OP_PLUS: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_add(Argument(REGISTER_RBX), Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
	} break;

	case OP_MINUS: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_sub(Argument(REGISTER_RBX), Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
	} break;

	case OP_MUL: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_imul(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
	} break;

	case OP_DIV: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(0)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_div(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RDX)));
	} break;

	case OP_EQUAL: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_cmove(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
	} break;

	case OP_GREATER: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_cmovg(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
	} break;

	case OP_LESS: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_cmovl(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
	} break;

	case OP_GREATER_EQ: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_cmovge(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
	} break;

	case OP_LESS_EQ: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_cmovle(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
	} break;

	case OP_NOT_EQ: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_cmovne(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
	} break;

	case OP_AND: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_and(Argument(REGISTER_RBX), Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
	} break;

	case OP_OR: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_or(Argument(REGISTER_RBX), Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
	} break;

	case OP_POP:
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		break;

	case OP_DUP:
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RSP, true, INFER_QWORD)));
		break;

	case OP_SWP: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
	} break;

	case OP_ROT: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RCX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
	} break;

	case OP_OVER: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
	} break;

	case OP_SET_VAR:
	case OP_SET_VAR_STRUCT_MEMBER:	{
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument("ret_stack_rsp", true)));
		asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
		handle_write_data_to_ptr(op);
	} break;

	case OP_SET_VAR_FROM_OTHER_PTR: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument("ret_stack_rsp", true)));
		asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
		handle_copy_ptr_to_ptr(op);
	} break;

	case OP_READ_VAR:
	case OP_READ_VAR_STRUCT_MEMBER: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument("ret_stack_rsp", true)));
		asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
		handle_read_data_from_ptr(op);
	} break;

	case OP_PUSH_VAR: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument("ret_stack_rsp", true)));
		asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
	} break;

	case OP_SET_PTR: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		handle_write_data_to_ptr(op);
	} break;

	case OP_SET_PTR_FROM_OTHER_PTR: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		handle_copy_ptr_to_ptr(op);
	} break;

	case OP_SET_PTR_STRUCT_MEMBER: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
		handle_write_data_to_ptr(op);
	} break;

	case OP_READ_PTR: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_xor(Argument(REGISTER_RBX), Argument(REGISTER_RBX)));
		handle_read_data_from_ptr(op);
	} break;

	case OP_READ_PTR_STRUCT_MEMBER: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
		handle_read_data_from_ptr(op);
	} break;

	case OP_SYSCALL0: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_syscall());
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
	} break;
	case OP_SYSCALL1: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
		asmp.new_inst(asmp.inst_syscall());
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
	} break;
	case OP_SYSCALL2: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RSI)));
		asmp.new_inst(asmp.inst_syscall());
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
	} break;
	case OP_SYSCALL3: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RSI)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDX)));
		asmp.new_inst(asmp.inst_syscall());
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
	} break;
	case OP_SYSCALL4: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RSI)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R10)));
		asmp.new_inst(asmp.inst_syscall());
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
	} break;
	case OP_SYSCALL5: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RSI)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R10)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R8)));
		asmp.new_inst(asmp.inst_syscall());
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
	} break;
	case OP_SYSCALL6: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RSI)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDX)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R10)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R8)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R9)));
		asmp.new_inst(asmp.inst_syscall());
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
	} break;

	case OP_LABEL:
	case OP_LABEL_END:
		asmp.new_inst(asmp.inst_label("addr_" + std::to_string(func_addr) + "_" + std::to_string(op.int_operand)));
		break;

	case OP_JMP:
	case OP_JMPE:
		asmp.new_inst(asmp.inst_jmp("addr_" + std::to_string(func_addr) + "_" + std::to_string(op.int_operand)));
		break;

	case OP_CJMPT:
	case OP_CJMPET: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_test(Argument(REGISTER_RAX), Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_jnz("addr_" + std::to_string(func_addr) + "_" + std::to_string(op.int_operand)));
	} break;

	case OP_CJMPF:
	case OP_CJMPEF: {
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_test(Argument(REGISTER_RAX), Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_jz("addr_" + std::to_string(func_addr) + "_" + std::to_string(op.int_operand)));
	} break;

	case OP_PUSH_INT: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(op.int_operand)));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
	} break;

	case OP_PUSH_STR: {
		asmp.string_data.push_back(op.str_operand);
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(op.str_operand.length())));
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
		asmp.new_inst(asmp.inst_push(Argument("str_" + std::to_string(asmp.string_data.size()-1))));
	} break;

	case OP_PUSH_TYPE_INSTANCE: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(9)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDI), Argument(0)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RSI), Argument(op.int_operand)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(3)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_R10), Argument(33)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_R8), Argument(-1)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_R9), Argument(0)));
		asmp.new_inst(asmp.inst_syscall());
		asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
	} break;

	case OP_DELETE_PTR: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(11)));
		asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RSI), Argument(op.int_operand)));
		asmp.new_inst(asmp.inst_syscall());
	} break;

	case OP_FUNCTION_CALL: {
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(REGISTER_RSP)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RSP), Argument("ret_stack_rsp", true)));
		asmp.new_inst(asmp.inst_call("func_addr_" + std::to_string(program.functions.at(op.str_operand).addr)));
		asmp.new_inst(asmp.inst_mov(Argument("ret_stack_rsp", true), Argument(REGISTER_RSP)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RSP), Argument(REGISTER_RAX)));
	} break;

	case OP_CONST:
	case OP_DEFINE_VAR:
	case OP_END:
	case OP_FUN:
	case OP_IMPORT:
	case OP_READ:
	case OP_SET:
	case OP_STRUCT:
	case OP_COUNT:
		print_error_at_loc(op.loc, "compiler error! this opcode (" + std::to_string(op.type) + ") shouldn't be passed into compiling stage");
		exit(1);
	}
}
