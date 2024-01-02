#include "../include/type_checker.h"

void TypeChecker::handle_arithmetic_op(Op op) {
	std::string fullname, name;

	switch (op.type) {
	case OP_PLUS:
		fullname = "addition";
		name = "+";
		break;
	case OP_MINUS:
		fullname = "subtraction";
		name = "-";
		break;
	case OP_MUL:
		fullname = "multiplication";
		name = "*";
		break;
	case OP_DIV:
		fullname = "division";
		name = "/";
		break;
	default:
		print_error_at_loc(op.loc, "This is an error in the type checker. Non-arithmetic opcode was sent to arithmetic handler");
		exit(1);
		break;
	} 
	
	if (type_stack.size() < 2) {
		print_not_enough_arguments_error(op.loc, 2, type_stack.size(), name, fullname);
		exit(1);
	}

	LangType a = type_stack.back(); type_stack.pop_back();
	LangType b = type_stack.back(); type_stack.pop_back();

	if (is_prim_type_int(a) && is_prim_type_int(b)) {
		type_stack.push_back(LangType(op.loc, prim_type_name(TYPE_I64), 0));
		if (op.type == OP_DIV)
			type_stack.push_back(LangType(op.loc, prim_type_name(TYPE_I64), 0));
	}
	else {
		print_invalid_combination_of_types_error(op.loc, {b, a}, name, fullname);
		print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b) + ")");
		print_note_at_loc(a.loc, "second value pushed here (" + human_readable_type(a) + ")");
		exit(1);
	}
}


void TypeChecker::handle_comparison_op(Op op) {
	std::string fullname = "", name;

	switch (op.type) {
	case OP_EQUAL:
		fullname = "equal to";
		name = "=";
		break;
	case OP_GREATER:
		fullname = "greater than";
		name = ">";
		break;
	case OP_LESS:
		fullname = "less than";
		name = "<";
		break;
	case OP_GREATER_EQ:
		fullname = "greater than or equal to";
		name = ">=";
		break;
	case OP_LESS_EQ:
		fullname = "less than or equal to";
		name = "<=";
		break;
	case OP_NOT_EQ:
		fullname = "not equal to";
		name = "<=";
		break;
	case OP_AND:
		name = "and";
		break;
	case OP_OR:
		name = "or";
		break;
	default:
		print_error_at_loc(op.loc, "This is an error in the type checker. Non-comparison opcode was sent to comparison handler");
		exit(1);
		break;
	}

	if (type_stack.size() < 2) {
		print_not_enough_arguments_error(op.loc, 2, type_stack.size(), name, fullname);
		exit(1);
	}
	
	LangType a = type_stack.back(); type_stack.pop_back();
	LangType b = type_stack.back(); type_stack.pop_back();

	if (types_equal(a, b))
		type_stack.push_back(LangType(
			op.loc, prim_type_name(TYPE_I64), 0
		));
	else {
		print_invalid_combination_of_types_error(op.loc, {b, a}, name, fullname);
		print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b) + ")");
		print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a) + ")");
		exit(1);
	}
}

void TypeChecker::handle_syscall_op(Op op) {
	std::string name;
	unsigned long int stack_size;

	switch (op.type) {
	case OP_SYSCALL0:
		name = "syscall0";
		stack_size = 1;
		break;
	case OP_SYSCALL1:
		name = "syscall1";
		stack_size = 2;
		break;
	case OP_SYSCALL2:
		name = "syscall2";
		stack_size = 3;
		break;
		break;
	case OP_SYSCALL3:
		name = "syscall3";
		stack_size = 4;
		break;
		break;
	case OP_SYSCALL4:
		name = "syscall4";
		stack_size = 5;
		break;
	case OP_SYSCALL5:
		name = "syscall5";
		stack_size = 6;
		break;
	case OP_SYSCALL6:
		name = "syscall6";
		stack_size = 7;
		break;
	default:
		print_error_at_loc(op.loc, "This is an error in the type checker. Non-syscall opcode was sent to syscall handler");
		exit(1);
		break;
	}

	if (type_stack.size() < stack_size) {
		print_not_enough_arguments_error(op.loc, stack_size, type_stack.size(), name);
		exit(1);
	}

	LangType a = type_stack.back(); type_stack.pop_back();
	for (unsigned long int i = 1; i < stack_size; i++)
		type_stack.pop_back();

	if (!is_prim_type_int(a)) {
		print_invalid_type_error(op.loc, prim_type_name(TYPE_I64), human_readable_type(a), name);
		print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a) + ")");
		exit(1);
	}
	type_stack.push_back(LangType(op.loc, prim_type_name(TYPE_I64), 0));
}

void TypeChecker::handle_jump_op(Op op) {
	std::string fullname, name;

	switch (op.type) {
	case OP_JMP:
	case OP_JMPE:
		jump_op_stack_states.push_back({op, type_stack});
		return;
		break;
	case OP_CJMPT:
		name = "cjmpt";
		fullname = "conditional jump if true";
		break;
	case OP_CJMPF:
		name = "cjmpf";
		fullname = "conditional jump if false";
		break;
	case OP_CJMPET:
		name = "cjmpet";
		fullname = "conditional jump to end if true";
		break;
	case OP_CJMPEF:
		name = "cjmpef";
		fullname = "conditional jump to end if false";
		break;
	default:
		print_error_at_loc(op.loc, "This is an error in the type checker. Non-jump opcode was sent to jump handler");
		exit(1);
		break;
	}

	if (type_stack.size() < 1) {
		print_not_enough_arguments_error(op.loc, 1, 0, name, fullname);
		exit(1);
	}
	LangType a = type_stack.back(); type_stack.pop_back();

	if (!is_prim_type_int(a)) {
		print_invalid_type_error(op.loc, prim_type_name(TYPE_I64), human_readable_type(a), name, fullname);
		print_note_at_loc(a.loc, "first argument found here (" + human_readable_type(a) + ")");
		exit(1);
	}
	jump_op_stack_states.push_back({op, type_stack});
}
