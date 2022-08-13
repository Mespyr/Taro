#include "include/parser.h"

Op convert_token_to_op(Token tok, Program program)
{
	static_assert(OP_COUNT == 12, "unhandled op types in convert_token_to_op()");

	if (tok.type == TOKEN_WORD)
	{
		if (tok.value == "dump")
			return Op(tok.loc, OP_DUMP);
		// arithmetic
		else if (tok.value == "+")
			return Op(tok.loc, OP_PLUS);
		else if (tok.value == "-")
			return Op(tok.loc, OP_MINUS);
		else if (tok.value == "*")
			return Op(tok.loc, OP_MUL);
		else if (tok.value == "/")
			return Op(tok.loc, OP_DIV);
		// keywords
		else if (tok.value == "fun")
			return Op(tok.loc, OP_FUN);
		else if (tok.value == "end")
			return Op(tok.loc, OP_END);
		else if (tok.value == "jmp")
			return Op(tok.loc, OP_JMP);
		else if (tok.value == "cjmp")
			return Op(tok.loc, OP_CJMP);
		// other
		else if (program.functions.count(tok.value))
			return Op(tok.loc, OP_FUNCTION_CALL, tok.value);
		else if (tok.value.back() == ':')
		{
			if (tok.value.size() == 1)
			{
				print_error_at_loc(tok.loc, "unexpected ':' char found while parsing");
				exit(1);
			}
			tok.value.pop_back();
			return Op(tok.loc, OP_LABEL, tok.value);
		}
	}
	else if (tok.type == TOKEN_INT)
		return Op(tok.loc, OP_PUSH_INT, atol(tok.value.c_str())); // TODO: check this
	else if (tok.type == TOKEN_STRING)
	{
		// TODO: implement strings
		print_error_at_loc(tok.loc, "strings are not supported yet");
		exit(1);
	}

	print_error_at_loc(tok.loc, "unknown word '" + tok.value + "'");
	exit(1);
}

std::vector<Op> link_ops(std::vector<Op> ops, std::map<std::string, int> labels)
{
	static_assert(OP_COUNT == 12, "unhandled op types in link_ops()");

	for (long unsigned int i = 0; i < ops.size(); i++)
	{
		Op current_op = ops.at(i);

		if (current_op.type == OP_JMP || current_op.type == OP_CJMP)
		{
			if (labels.count(current_op.str_operand))
			{
				current_op.int_operand = labels.at(current_op.str_operand);
				ops.at(i) = current_op;
			}
			else
			{
				print_error_at_loc(current_op.loc, "label '" + current_op.str_operand + "' not found");
				exit(1);
			}
		}
	}

	return ops;
}

Program parse_tokens(std::vector<Token> tokens)
{
	static_assert(OP_COUNT == 12, "unhandled op types in parse_tokens()");

	Program program;
	long unsigned int i = 0;
	int function_addr = 0;

	while (i < tokens.size())
	{
		Op current_op = convert_token_to_op(tokens.at(i), program);
		
		if (current_op.type == OP_FUN)
		{
			i++;
			if (i > tokens.size() - 2)
			{
				print_error_at_loc(current_op.loc, "unexpected EOF found while parsing function definition");
				exit(1);
			}
		
			// check function name
			Token name_token = tokens.at(i);
			std::string function_name = name_token.value;

			if (name_token.type == TOKEN_WORD)
			{
				if (is_builtin_word(function_name))
				{
					print_error_at_loc(name_token.loc, "function name cannot be a built-in word");
					exit(1);
				}
				else if (program.functions.count(function_name))
				{
					print_error_at_loc(name_token.loc, "function '" + function_name + "' already exists");
					exit(1);
				}
			}
			else if (name_token.type == TOKEN_INT)
			{
				print_error_at_loc(current_op.loc, "function name cannot be a integer");
				exit(1);
			}
			else if (name_token.type == TOKEN_STRING)
			{
				print_error_at_loc(current_op.loc, "function name cannot be a string");
				exit(1);
			}
		
			// check if there is code after function declaration
			i++;
			if (i >= tokens.size())
			{
				print_error_at_loc(tokens.back().loc, "unexpected EOF found while parsing function definition");
				exit(1);
			}

			program.functions.insert({function_name, Function(
				name_token.loc, function_addr
			)});
			function_addr++;

			std::vector<Op> function_ops;
			std::map<std::string, int> labels;
			bool found_function_end = false;
		
			// parse tokens in function
			while (i < tokens.size())
			{
				Op f_op = convert_token_to_op(tokens.at(i), program);
				
				if (f_op.type == OP_FUN)
				{
					print_error_at_loc(f_op.loc, "unexpected 'sec' keyword found while parsing. functions cannot be defined inside other functions");
					exit(1);
				}
				else if (f_op.type == OP_END)
				{
					found_function_end = true;
					break;
				}
				else if (f_op.type == OP_LABEL)
				{
					if (labels.count(f_op.str_operand))
					{
						print_error_at_loc(f_op.loc, "redefinition of label '" + f_op.str_operand + "'");
						exit(1);
					}
					labels.insert({f_op.str_operand, i});
					f_op.int_operand = i;
					function_ops.push_back(f_op);
				}
				else if (f_op.type == OP_JMP || f_op.type == OP_CJMP)
				{
					i++;
					if (i >= tokens.size()) break;

					Token label_jumpto_tok = tokens.at(i);
					f_op.str_operand = label_jumpto_tok.value;
					function_ops.push_back(f_op);
				}
				else function_ops.push_back(f_op);

				i++;
			}
			if (found_function_end)
				program.functions.at(function_name).ops = link_ops(function_ops, labels);
			else
			{
				print_error_at_loc(tokens.back().loc, "Unexpected EOF while parsing function body");
				exit(1);
			}
		}
		else
		{
			print_error_at_loc(current_op.loc, "Unexpected token found while parsing");
			exit(1);
		}

		i++;
	}

	return program;
}
