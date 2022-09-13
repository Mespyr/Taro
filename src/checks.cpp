#include "include/checks.h"

void verify_program(Program program)
{
	if (!program.functions.count("main"))
	{
		print_error("no entry point found in program (no 'main' function)");
		exit(1);
	}

	Function main_func = program.functions.at("main");
	if (main_func.arg_stack.size() > 0)
	{
		print_error_at_loc(main_func.loc, "'main' function must not pass any arguments");
		exit(1);
	}

	if (main_func.ret_stack.size() > 0)
	{
		print_error_at_loc(main_func.loc, "'main' function must not have any return values");
		exit(1);
	}
}

bool compare_type_stacks(std::vector<TypeAtLoc> type_stack_1, std::vector<TypeAtLoc> type_stack_2)
{
	if (type_stack_1.size() != type_stack_2.size())
		return false;

	for (long unsigned int i = 0; i < type_stack_1.size(); i++)
	{
		DATATYPE type_1 = type_stack_1.at(i).type;
		DATATYPE type_2 = type_stack_2.at(i).type;
		if (type_1 != type_2)
			return false;
	}

	return true;
}

// TODO: make it so that stack state on jump ops is stored in a map
// then checked over with the second part of type-checking
// comparing it to the label_stack_states map
void type_check_program(Program program)
{
	static_assert(OP_COUNT == 39, "unhandled op types in type_check_program()");
	static_assert(DATATYPE_COUNT == 2, "unhandled datatypes in type_check_program()");

	for (auto fn_key = program.functions.begin(); fn_key != program.functions.end(); fn_key++)
	{
		Function function = fn_key->second;
		std::string func_name = fn_key->first;
		std::map<std::string, std::vector<TypeAtLoc>> label_stack_states;
		std::vector<std::pair<Op, std::vector<TypeAtLoc>>> jump_op_stack_states;

		// first round of type-checking
		// get all type_stack snapshots of all label sections
		std::vector<TypeAtLoc> type_stack;

		for (TypeAtLoc t : function.arg_stack)
			type_stack.push_back(t);

		// type check all ops
		for (Op op : function.ops)
		{
			if (op.type == OP_DUMP)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "dump");
					exit(1);
				}
				type_stack.pop_back();
			}

			// arithmetics
			else if (op.type == OP_PLUS)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "+", "addition");
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				// additions goes in following combinations [b, a] -> [b + a]

				// int + int -> int
				if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				// ptr + int -> ptr
				else if (a.type == DATATYPE_INT && b.type == DATATYPE_PTR)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
				// int + ptr -> ptr
				else if (a.type == DATATYPE_PTR && b.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "+", "addition");
					print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_MINUS)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "-", "subtraction");
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				// subtraction goes in following combinations [b, a] -> [b - a]

				// int - int -> int
				if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				// ptr - int -> ptr
				else if (a.type == DATATYPE_INT && b.type == DATATYPE_PTR)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
				// ptr - ptr -> int
				else if (a.type == DATATYPE_PTR && b.type == DATATYPE_PTR)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "-", "subtraction");
					print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_MUL)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "*", "multiplication");
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				// multiplication goes in following combinations

				// int * int -> int
				if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "*", "multiplication");
					print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_DIV)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "/", "division");
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				// division goes in following combinations

				// int / int -> int, int
				if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
				{
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				}
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "/", "division");
					print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}

			// comparisons
			else if (op.type == OP_EQUAL)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "=", "equal to");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "=", "equal to");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_GREATER)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), ">", "greater than");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, ">", "greater than");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_LESS)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "<", "less than");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "<", "less than");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_GREATER_EQ)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), ">=", "greater than or equal to");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, ">=", "greater than or equal to");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_LESS_EQ)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "<=", "less than or equal to");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, ">=", "less than or equal to");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_NOT_EQ)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "!=", "not equal to");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "!=", "not equal to");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_NOT)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "not");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();

				if (a.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "not");
					print_note_at_loc(a.loc, "first argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_AND)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "and");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "and");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_OR)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "or");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "or");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}		

			// stack manipulation
			else if (op.type == OP_POP)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "pop");
					exit(1);
				}
				type_stack.pop_back();
			}
			else if (op.type == OP_DUP)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "dup");
					exit(1);
				}
				TypeAtLoc a = type_stack.back();
				a.loc = op.loc;
				type_stack.push_back(a);
			}
			else if (op.type == OP_SWP)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "swp");
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();
				type_stack.push_back(a);
				type_stack.push_back(b);
			}
			else if (op.type == OP_ROT)
			{
				if (type_stack.size() < 3)
				{
					print_not_enough_arguments_error(op.loc, 3, type_stack.size(), "rot", "rotate");
					exit(1);
				}

				// [c, b, a] -> [b, a, c]
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();
				TypeAtLoc c = type_stack.back(); type_stack.pop_back();
				type_stack.push_back(b);
				type_stack.push_back(a);
				type_stack.push_back(c);
			}
			else if (op.type == OP_OVER)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "over");
					exit(1);
				}
				
				// [b, a] -> [b, a, b]
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();
				type_stack.push_back(b);
				type_stack.push_back(a);
				b.loc = op.loc;
				type_stack.push_back(b);
			}

			// syscalls
			else if (op.type == OP_SYSCALL0)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "syscall0");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall0");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL1)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "syscall1");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall1");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL2)
			{
				if (type_stack.size() < 3)
				{
					print_not_enough_arguments_error(op.loc, 3, type_stack.size(), "syscall2");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall2");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL3)
			{
				if (type_stack.size() < 4)
				{
					print_not_enough_arguments_error(op.loc, 4, type_stack.size(), "syscall3");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall3");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL4)
			{
				if (type_stack.size() < 5)
				{
					print_not_enough_arguments_error(op.loc, 5, type_stack.size(), "syscall4");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall4");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL5)
			{
				if (type_stack.size() < 6)
				{
					print_not_enough_arguments_error(op.loc, 6, type_stack.size(), "syscall5");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall5");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL6)
			{
				if (type_stack.size() < 7)
				{
					print_not_enough_arguments_error(op.loc, 7, type_stack.size(), "syscall6");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall6");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}

			// labels
			// we do not check OP_JMP and OP_JMPE as they don't consume any data from the stack
			// and we will compare the type stack to the labels in the second loop
			else if (op.type == OP_LABEL)
			{
				// save current stack state
				label_stack_states.insert({op.str_operand, type_stack});
			}
			else if (op.type == OP_LABEL_END)
			{
				if (!compare_type_stacks(type_stack, label_stack_states.at(op.str_operand)))
				{
					print_error_at_loc(op.loc, "different types on stack before and after label definition. types of items on stack must be the same.");
					exit(1);
				}
			}
			else if (op.type == OP_JMP)
			{
				jump_op_stack_states.push_back({op, type_stack});
			}
			else if (op.type == OP_CJMPT)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "cjmpt", "conditional jump if true");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "cjmpt", "conditional jump if true");
					print_note_at_loc(a.loc, "first argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}

				jump_op_stack_states.push_back({op, type_stack});
			}
			else if (op.type == OP_CJMPF)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "cjmpf", "conditional jump if false");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "cjmpf", "conditional jump if false");
					print_note_at_loc(a.loc, "first argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}

				jump_op_stack_states.push_back({op, type_stack});
			}
			else if (op.type == OP_CJMPET)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "cjmpet", "conditional jump to end if true");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "cjmpet", "conditional jump to end if true");
					print_note_at_loc(a.loc, "first argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}

				jump_op_stack_states.push_back({op, type_stack});
			}
			else if (op.type == OP_CJMPEF)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "cjmpef", "conditional jump to end if false");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "cjmpet", "conditional jump to end if false");
					print_note_at_loc(a.loc, "first argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}

				jump_op_stack_states.push_back({op, type_stack});
			}

			// other
			else if (op.type == OP_PUSH_INT)
			{
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_PUSH_STR)
			{
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
			}
			else if (op.type == OP_FUNCTION_CALL)
			{
				assert(program.functions.count(op.str_operand));

				Function call_func = program.functions.at(op.str_operand);

				if (type_stack.size() < call_func.arg_stack.size())
				{
					print_not_enough_arguments_error(op.loc, call_func.arg_stack.size(), type_stack.size(), op.str_operand, "", true);
					exit(1);
				}

				std::vector<TypeAtLoc> args;
				for (unsigned long int i = call_func.arg_stack.size(); i > 0; i--)
				{
					args.push_back(type_stack.back());
					type_stack.pop_back();
				}
				std::reverse(args.begin(), args.end());

				bool args_match_types = compare_type_stacks(args, call_func.arg_stack);
				if (!args_match_types)
				{
					if (call_func.arg_stack.size() == 1)
						print_invalid_type_error(op.loc, call_func.arg_stack.at(0).type, args.at(0).type, op.str_operand, "", true);
					else
					{
						std::vector<DATATYPE> types;
						for (TypeAtLoc t : args)
							types.push_back(t.type);

						print_invalid_combination_of_types_error(op.loc, types, op.str_operand, "", true);
						for (TypeAtLoc t : args)
							print_note_at_loc(t.loc, "argument pushed here (" + human_readable_type(t.type) + ")");
					}
					exit(1);
				}

				for (TypeAtLoc t : call_func.ret_stack)
					type_stack.push_back(t);
			}

			// unreachable
			else if (op.type == OP_FUN || op.type == OP_END || op.type == OP_COUNT)
			{
				print_error_at_loc(op.loc, "unreachable: op should be handled in the parsing step. This is probably a bug.");
				exit(1);
			}
		}

		// make sure return values match up with what is specified in the function definition and throw error if not
		if (!compare_type_stacks(type_stack, function.ret_stack))
		{
			if (function.ret_stack.size() < type_stack.size()) // main cannot have excess data on stack
			{
				print_error_at_loc(function.loc, "unhandled data on the stack (expected " + std::to_string(function.ret_stack.size()) + " items, got " + std::to_string(type_stack.size()) + ")");

				std::vector<TypeAtLoc> excess_stack;
				for (unsigned long int i = type_stack.size() - function.ret_stack.size(); i > 0; i--)
				{
					excess_stack.push_back(type_stack.back());
					type_stack.pop_back();
				}
				std::reverse(excess_stack.begin(), excess_stack.end());

				for (TypeAtLoc t : excess_stack)
					print_note_at_loc(t.loc, "excess data pushed here (" + human_readable_type(t.type) + ")");
			}

			else if (function.ret_stack.size() > type_stack.size())
				print_error_at_loc(function.loc, "not enough data on the stack (expected " + std::to_string(function.ret_stack.size()) + " items, got " + std::to_string(type_stack.size()) + ")");

			else if (type_stack.size() > 1)
			{
				std::vector<DATATYPE> types;
				for (TypeAtLoc t : type_stack)
					types.push_back(t.type);
				print_invalid_combination_of_types_error(function.loc, types, func_name, "", true);
				for (TypeAtLoc t : type_stack)
					print_note_at_loc(t.loc, "argument pushed here (" + human_readable_type(t.type) + ")");
			}

			else
			{
				print_invalid_type_error(function.loc, function.ret_stack.at(0).type, type_stack.at(0).type, func_name, "", true);
				print_note_at_loc(type_stack.at(0).loc, "value pushed here (" + human_readable_type(type_stack.at(0).type) + ")");
			}

			exit(1);
		}

		// second round of type-checking
		// go through code again and check jmp labels to see if the stack values are the same as the label they are jumping to
		for (std::pair<Op, std::vector<TypeAtLoc>> op_stack_pair : jump_op_stack_states)
		{
			Op op = op_stack_pair.first;
			std::vector<TypeAtLoc> op_stack_state = op_stack_pair.second;

			if (!label_stack_states.count(op.str_operand))
			{
				print_error_at_loc(op.loc, "error found in parser or type-checker. Label '" + op.str_operand + " either doesn't exist or hasn't been found by the parser.");
				exit(1);
			}

			// throw error if stack doesn't match up
			if (!compare_type_stacks(op_stack_state, label_stack_states.at(op.str_operand)))
			{
				print_error_at_loc(op.loc, "different types on stack then what was expected at the label.");

				exit(1);
			}
		}
	}
}
