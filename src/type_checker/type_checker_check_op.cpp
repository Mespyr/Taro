#include "type_checker.h"

void TypeChecker::check_op() {
	Op op = function->ops.at(idx);

	switch (op.type) {

	case OP_DUMP: {
		if (type_stack.size() < 1) {
			print_not_enough_arguments_error(op.loc, 1, 0, "dump");
			exit(1);
		}
		type_stack.pop_back();
	} break;

	case OP_PLUS:
	case OP_MINUS:
	case OP_MUL:
	case OP_DIV:
		handle_arithmetic_op(op);
		break;

	case OP_EQUAL:
	case OP_GREATER:
	case OP_LESS:
	case OP_GREATER_EQ:
	case OP_LESS_EQ:
	case OP_NOT_EQ:
	case OP_AND:
	case OP_OR:
		handle_comparison_op(op);
		break;

	case OP_POP: {
		if (type_stack.size() < 1) {
			print_not_enough_arguments_error(op.loc, 1, 0, "pop");
			exit(1);
		}
		type_stack.pop_back();
	} break;

	case OP_DUP: {
		if (type_stack.size() < 1) {
			print_not_enough_arguments_error(op.loc, 1, 0, "dup");
			exit(1);
		}
		LangType a = type_stack.back();
		a.loc = op.loc;
		type_stack.push_back(a);
	} break;

	case OP_SWP: {
		if (type_stack.size() < 2) {
			print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "swp");
			exit(1);
		}
		LangType a = type_stack.back(); type_stack.pop_back();
		LangType b = type_stack.back(); type_stack.pop_back();
		type_stack.push_back(a);
		type_stack.push_back(b);
	} break;

	case OP_ROT: {
		if (type_stack.size() < 3) {
			print_not_enough_arguments_error(op.loc, 3, type_stack.size(), "rot", "rotate");
			exit(1);
		}
		// [c, b, a] -> [b, a, c]
		LangType a = type_stack.back(); type_stack.pop_back();
		LangType b = type_stack.back(); type_stack.pop_back();
		LangType c = type_stack.back(); type_stack.pop_back();
		type_stack.push_back(b);
		type_stack.push_back(a);
		type_stack.push_back(c);
	} break;

	case OP_OVER: {
		if (type_stack.size() < 2) {
			print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "over");
			exit(1);
		}
		// [b, a] -> [b, a, b]
		LangType a = type_stack.back(); type_stack.pop_back();
		LangType b = type_stack.back(); type_stack.pop_back();
		type_stack.push_back(b);
		type_stack.push_back(a);
		b.loc = op.loc;
		type_stack.push_back(b);
	} break;

	case OP_SET_VAR: {
		static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in type_check_program()");
		if (type_stack.size() < 1) {
			print_not_enough_arguments_error(op.loc, 1, 0, "&", "set variable");
			exit(1);
		}
		LangType a = type_stack.back(); type_stack.pop_back();
		LangType expected_type = function->var_offsets.at(op.str_operand).first;

		if (op.is_prim_type_mode()) {
			if (!types_equal(a, expected_type)) {
				// check in value on stack is a pointer to the said value (a variable)
				expected_type.ptr_to_trace++;
				if (!types_equal(a, expected_type)) {
					expected_type.ptr_to_trace--;
					print_invalid_type_error(op.loc, human_readable_type(expected_type), human_readable_type(a), "@", "set variable");
					exit(1);
				}
				op.type = OP_SET_VAR_FROM_OTHER_PTR;
				program.functions.at(func_name).ops.at(idx) = op;
			}
		}
		else if (op.mode == MODE_STRUCT) {
			expected_type.ptr_to_trace++;
			// a should be a pointer to the expected_type
			if (a.base_type != expected_type.base_type || a.ptr_to_trace != expected_type.ptr_to_trace) {
				print_error_at_loc(op.loc, "Cannot set '" + op.str_operand + "' to type '" + human_readable_type(a) + "'. Expected type '" + human_readable_type(expected_type) + "'");
				exit(1);
			}
		}
	} break;

	case OP_SET_VAR_STRUCT_MEMBER: {
		static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in type_check_program()");
		if (type_stack.size() < 1) {
			print_not_enough_arguments_error(op.loc, 1, 0, "@", "set struct member");
			exit(1);
		}
		LangType a = type_stack.back(); type_stack.pop_back();
		std::pair<LangType, int> member_type_offset = variable_member_offset(op, function->var_offsets, program.structs);
		// the type needs to be a pointer to a struct
		// op-type OP_SET_MEMBER_STRUCT assumes the value was a struct (no pointers)
		if (op.mode == MODE_STRUCT)
			member_type_offset.first.ptr_to_trace += 1;
		if (a.base_type != member_type_offset.first.base_type && a.ptr_to_trace != member_type_offset.first.ptr_to_trace) {
			print_error_at_loc(op.loc, "Cannot set '" + op.str_operand + "' to type '" + human_readable_type(a) + "'. Expected type '" + human_readable_type(member_type_offset.first) + "'");
			exit(1);
		}
	} break;

	case OP_READ_VAR: {
		// if it is a primitive type sizes as only primitive types can be read directly
		if (op.is_prim_type_mode()) {
			LangType t = function->var_offsets.at(op.str_operand).first;
			t.loc = op.loc;
			type_stack.push_back(t);
		}
		else {
			print_error_at_loc(op.loc, "compiler error: op OP_READ_VAR is set in a non-primitive mode, this is probably a bug in the parser");
			exit(1);
		}
	} break;

	case OP_READ_VAR_STRUCT_MEMBER: {
		static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in type_check_program()");
		std::pair<LangType, int> member_type_offset = variable_member_offset(op, function->var_offsets, program.structs);
		member_type_offset.first.loc = op.loc;
		if (op.mode == MODE_STRUCT)
			member_type_offset.first.ptr_to_trace++;
		type_stack.push_back(member_type_offset.first);
	} break;

	case OP_PUSH_VAR:
		type_stack.push_back(LangType(op.loc,
			function->var_offsets.at(op.str_operand).first.base_type,
			function->var_offsets.at(op.str_operand).first.ptr_to_trace + 1));
		break;

	case OP_SET_PTR: {
		static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in type_check_program()");
		if (type_stack.size() < 2) {
			print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "@", "set pointer");
			exit(1);
		}
		LangType a = type_stack.back(); type_stack.pop_back(); // ptr
		LangType b = type_stack.back(); type_stack.pop_back(); // value
		LangType expected_type(op.loc, op.str_operand);

		expected_type.ptr_to_trace++;
		if (!types_equal(expected_type, a)) {
			print_invalid_type_error(op.loc, human_readable_type(expected_type), human_readable_type(a), "@", "set pointer");
			exit(1);
		}
		expected_type.ptr_to_trace--;

		if (op.is_prim_type_mode()) {
			if (!types_equal(b, expected_type)) {
				// check in value on stack is a pointer to the said value (a variable)
				expected_type.ptr_to_trace++;
				if (!types_equal(b, expected_type)) {
					expected_type.ptr_to_trace--;
					print_invalid_type_error(op.loc, human_readable_type(expected_type), human_readable_type(b), "@", "set pointer");
					exit(1);
				}
				op.type = OP_SET_PTR_FROM_OTHER_PTR;
				program.functions.at(func_name).ops.at(idx) = op;
			}
		}
		else if (op.mode == MODE_STRUCT) {
			expected_type.ptr_to_trace++;
			// a should be a pointer to the expected_type, print error if not
			if (a.base_type != expected_type.base_type || a.ptr_to_trace != expected_type.ptr_to_trace) {
				print_error_at_loc(op.loc, "Cannot set pointer of type '" + op.str_operand + "' to type '" + human_readable_type(a) + "'. Expected type '" + human_readable_type(expected_type) + "'");
				exit(1);
			}
		}
	} break;

	case OP_SET_PTR_STRUCT_MEMBER: {
		static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in type_check_program()");
		if (type_stack.size() < 2) {
			print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "@", "set pointer member");
			exit(1);
		}
		LangType a = type_stack.back(); type_stack.pop_back(); // ptr
		LangType b = type_stack.back(); type_stack.pop_back(); // value
		LangType expected_type(op.loc, split_by_dot(op.str_operand).front());
		
		expected_type.ptr_to_trace++;
		if (!types_equal(expected_type, a)) {
			print_invalid_type_error(op.loc, human_readable_type(expected_type), human_readable_type(a), "@", "set pointer member");
			exit(1);
		}
		expected_type.ptr_to_trace--;

		std::pair<LangType, int> member_type_offset = struct_member_offset(op, program.structs);
		if (op.mode == MODE_STRUCT)
			member_type_offset.first.ptr_to_trace++;
		if (!types_equal(b, member_type_offset.first)) {
			print_error_at_loc(op.loc, "cannot set pointer of type '" + op.str_operand + "' to type '" + human_readable_type(b) + "'. Expected type '" + human_readable_type(member_type_offset.first) + "'");
			exit(1);
		}
	} break;

	case OP_READ_PTR: {
		static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in type_check_program()");
		if (type_stack.size() < 1) {
			print_not_enough_arguments_error(op.loc, 1, 0, "&", "read pointer");
			exit(1);
		}
		LangType a = type_stack.back(); type_stack.pop_back();
		LangType expected_type(op.loc, op.str_operand);

		expected_type.ptr_to_trace++;
		if (!types_equal(expected_type, a)) {
			print_invalid_type_error(op.loc, human_readable_type(expected_type), human_readable_type(a), "&", "read pointer");
			exit(1);
		}
		expected_type.ptr_to_trace--;
		type_stack.push_back(expected_type);
	} break;

	case OP_READ_PTR_STRUCT_MEMBER: {
		static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in type_check_program()");
		if (type_stack.size() < 1) {
			print_not_enough_arguments_error(op.loc, 1, 0, "&", "read pointer member");
			exit(1);
		}
		LangType a = type_stack.back(); type_stack.pop_back();
		LangType expected_type(op.loc, split_by_dot(op.str_operand).front()); // get type of struct we are getting the member from

		expected_type.ptr_to_trace++;
		if (!types_equal(expected_type, a)) {
			print_invalid_type_error(op.loc, human_readable_type(expected_type), human_readable_type(a), "&", "read pointer member");
			exit(1);
		}
		expected_type.ptr_to_trace--;

		std::pair<LangType, int> member_type_offset = struct_member_offset(op, program.structs);
		member_type_offset.first.loc = op.loc;
		if (op.mode == MODE_STRUCT)
			member_type_offset.first.ptr_to_trace++;
		type_stack.push_back(member_type_offset.first);
	} break;

	case OP_SYSCALL0:
	case OP_SYSCALL1:
	case OP_SYSCALL2:
	case OP_SYSCALL3:
	case OP_SYSCALL4:
	case OP_SYSCALL5:
	case OP_SYSCALL6:
		handle_syscall_op(op);
		break;

	case OP_LABEL:
		label_stack_states.insert({op.str_operand, type_stack});
		break;

	case OP_LABEL_END: 
		if (!compare_type_stacks(type_stack, label_stack_states.at(op.str_operand))) {
			print_error_at_loc(op.loc, "different types on stack before and after label definition. types of items on stack must be the same.");
			exit(1);
		}
		break;

	case OP_JMP:
	case OP_JMPE:
	case OP_CJMPT:
	case OP_CJMPF:
	case OP_CJMPET:
	case OP_CJMPEF:
		handle_jump_op(op);
		break;

	case OP_PUSH_INT:
		type_stack.push_back(LangType(op.loc, prim_type_name(TYPE_I64), 0));
		break;

	case OP_PUSH_STR:
		type_stack.push_back(LangType(op.loc, prim_type_name(TYPE_I64), 0));
		type_stack.push_back(LangType(op.loc, prim_type_name(TYPE_I8), 1)); // pointer to array of ints (string)
		break;

	case OP_PUSH_TYPE_INSTANCE: {
		LangType t(op.loc, op.str_operand);
		t.ptr_to_trace++;
		type_stack.push_back(t);
	} break;

	case OP_DELETE_PTR: {
		if (type_stack.size() < 1) {
			print_not_enough_arguments_error(op.loc, 1, 0, "delete", "delete pointer");
			exit(1);
		}
		LangType t = type_stack.back(); type_stack.pop_back();
		if (t.ptr_to_trace == 0) {
			print_error_at_loc(op.loc, "Can't delete a non-pointer");
			exit(1);
		}
		op.int_operand = sizeof_type(t, program.structs);
		program.functions.at(func_name).ops.at(idx) = op;
	} break;

	case OP_FUNCTION_CALL: {
		assert(program.functions.count(op.str_operand));

		Function call_func = program.functions.at(op.str_operand);

		if (type_stack.size() < call_func.signature.argument_stack.size()) {
			print_not_enough_arguments_error(op.loc, call_func.signature.argument_stack.size(), type_stack.size(), op.str_operand, "", true);
			exit(1);
		}

		std::vector<LangType> args;
		for (unsigned long int i = call_func.signature.argument_stack.size(); i > 0; i--) {
			args.push_back(type_stack.back());
			type_stack.pop_back();
		}
		std::reverse(args.begin(), args.end());

		bool args_match_types = compare_type_stacks(args, call_func.signature.argument_stack);
		if (!args_match_types) {
			if (call_func.signature.argument_stack.size() == 1)
				print_invalid_type_error(op.loc, human_readable_type(call_func.signature.argument_stack.at(0)), human_readable_type(args.at(0)), op.str_operand, "", true);
			else {
				print_invalid_combination_of_types_error(op.loc, args, op.str_operand, "", true);
				for (LangType t : args)
					print_note_at_loc(t.loc, "argument pushed here (" + human_readable_type(t) + ")");
			}
			exit(1);
		}

		for (LangType t : call_func.signature.return_stack)
			type_stack.push_back(t);
	} break;

	case OP_FUN:
	case OP_END:
	case OP_STRUCT:
	case OP_DEFINE_VAR:
	case OP_CONST:
	case OP_IMPORT:
	case OP_SET:
	case OP_READ:
	case OP_COUNT:
		print_error_at_loc(op.loc, "unreachable: op should be handled in the parsing step. This is probably a bug.");
		exit(1);
		break;

	case OP_SET_PTR_FROM_OTHER_PTR:
	case OP_SET_VAR_FROM_OTHER_PTR:
		print_error_at_loc(op.loc, "unreachable: op is created in type-checking step and shouldn't be passed in to be type-checked. This is probably a bug.");
		exit(1);
		break;
	
	}
}
