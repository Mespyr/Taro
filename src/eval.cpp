#include "include/eval.h"

ConstExpr eval_const_expression(Program program, std::vector<Token> tokens, long unsigned int i, Location definition_loc)
{
	std::vector<long long> stack;

	while (tokens.at(i).value != "end")
	{
		Op op = convert_token_to_op(tokens.at(i), program);

		if (op.type == OP_PUSH_INT)
			stack.push_back(op.int_operand);
		else if (op.type == OP_PLUS)
		{
			if (stack.size() < 2)
			{
				print_not_enough_arguments_error(op.loc, 2, stack.size(), "+", "addition");
				exit(1);
			}
			long long a = stack.back(); stack.pop_back();
			long long b = stack.back(); stack.pop_back();
			stack.push_back(a + b);
		}
		else if (op.type == OP_MINUS)
		{
			if (stack.size() < 2)
			{
				print_not_enough_arguments_error(op.loc, 2, stack.size(), "-", "subtraction");
				exit(1);
			}
			// b, a = b - a
			long long a = stack.back(); stack.pop_back();
			long long b = stack.back(); stack.pop_back();
			stack.push_back(b - a);
		}
		else if (op.type == OP_MUL)
		{
			if (stack.size() < 2)
			{
				print_not_enough_arguments_error(op.loc, 2, stack.size(), "*", "multiplication");
				exit(1);
			}
			long long a = stack.back(); stack.pop_back();
			long long b = stack.back(); stack.pop_back();
			stack.push_back(a * b);
		}
		else if (op.type == OP_DIV)
		{
			if (stack.size() < 2)
			{
				print_not_enough_arguments_error(op.loc, 2, stack.size(), "/", "division");
				exit(1);
			}
			// b, a = b/a, b%a
			long long a = stack.back(); stack.pop_back();
			long long b = stack.back(); stack.pop_back();
			stack.push_back(b / a);
			stack.push_back(b % a);
		}
		else if (op.type == OP_POP)
		{
			if (stack.size() < 1)
			{
				print_not_enough_arguments_error(op.loc, 1, stack.size(), "pop");
				exit(1);
			}
			stack.pop_back();
		}
		else if (op.type == OP_SWP)
		{
			if (stack.size() < 2)
			{
				print_not_enough_arguments_error(op.loc, 2, stack.size(), "swp");
				exit(1);
			}
			// b, a = a, b
			long long a = stack.back(); stack.pop_back();
			long long b = stack.back(); stack.pop_back();
			stack.push_back(a);
			stack.push_back(b);
		}
		else
		{
			print_error_at_loc(op.loc, "unsuppored keyword in compile-time evaluation of expression");
			exit(1);
		}

		i++;
		if (i > tokens.size() - 1)
		{
			print_error_at_loc(tokens.at(i - 1).loc, "unexpected EOF found while parsing");
			exit(1);
		}
	}

	if (stack.size() > 1)
	{
		print_error_at_loc(definition_loc, "too many values on stack for definition (expected 1, got " + std::to_string(stack.size()));
		exit(1);
	}
	else if (stack.size() < 1)
	{
		print_error_at_loc(definition_loc, "not enough values on stack for definition (expected 1, got 0)");
		exit(1);
	}
	return ConstExpr {stack.back(), i};
}
