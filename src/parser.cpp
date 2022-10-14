#include "include/parser.h"

bool is_legal_name(Token token_name)
{
	static_assert(BASE_TYPES_COUNT == 2, "unhandled base types in is_legal_name()");

	if (token_name.type == TOKEN_INT || token_name.type == TOKEN_STRING) return false;
	if (is_builtin_word(token_name.value)) return false;
	if (token_name.value.find('"') != std::string::npos) return false;

	std::string base_type = parse_type_str(token_name.value).first;
	if (base_type == get_base_type_name(TYPE_I64) || 
		base_type == get_base_type_name(TYPE_I8))
		return false;

	return true;
}

std::string add_escapes_to_string(std::string str)
{
	std::string buf;
	std::string ret;
	long unsigned int i = 0;

	// can't get 2 chars if string is 1 or 0 chars
	if (str.length() < 2) return str;

	while (i < str.length())
	{
		buf = str.substr(i, 2);

		if (buf == "\\a")       ret.push_back('\a');
		else if (buf == "\\b")  ret.push_back('\b');
		else if (buf == "\\f")  ret.push_back('\f');
		else if (buf == "\\n")  ret.push_back('\n');
		else if (buf == "\\r")  ret.push_back('\r');
		else if (buf == "\\t")  ret.push_back('\t');
		else if (buf == "\\v")  ret.push_back('\v');
		else if (buf == "\\\\") ret.push_back('\\');
		else if (buf == "\\'")  ret.push_back('\'');
		else if (buf == "\\\"") ret.push_back('\"');
		else if (buf == "\\\?") ret.push_back('\?');
		else if (buf == "\\0")  ret.push_back('\0');
		else
		{
			// if escape sequence not found, shift buffer over by one char to next section
			ret.push_back(buf.at(0));
			i++;
			continue;
		}

		// skip both chars if escape sequence found in buffer
		i+=2;
	}

	return ret;
}

Op convert_token_to_op(Token tok, Program program, std::map<std::string, std::pair<LCPType, int>> var_offsets)
{
	static_assert(OP_COUNT == 52, "unhandled op types in convert_token_to_op()");

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
		// comparisons
		else if (tok.value == "=")
			return Op(tok.loc, OP_EQUAL);
		else if (tok.value == ">")
			return Op(tok.loc, OP_GREATER);
		else if (tok.value == "<")
			return Op(tok.loc, OP_LESS);
		else if (tok.value == ">=")
			return Op(tok.loc, OP_GREATER_EQ);
		else if (tok.value == "<=")
			return Op(tok.loc, OP_LESS_EQ);
		else if (tok.value == "!=")
			return Op(tok.loc, OP_NOT_EQ);
		else if (tok.value == "not")
			return Op(tok.loc, OP_NOT);
		else if (tok.value == "and")
			return Op(tok.loc, OP_AND);
		else if (tok.value == "or")
			return Op(tok.loc, OP_OR);
		// stack manipulation
		else if (tok.value == "pop")
			return Op(tok.loc, OP_POP);
		else if (tok.value == "dup")
			return Op(tok.loc, OP_DUP);
		else if (tok.value == "swp")
			return Op(tok.loc, OP_SWP);
		else if (tok.value == "rot")
			return Op(tok.loc, OP_ROT);
		else if (tok.value == "over")
			return Op(tok.loc, OP_OVER);
		// keywords
		else if (tok.value == "fun")
			return Op(tok.loc, OP_FUN);
		else if (tok.value == "end")
			return Op(tok.loc, OP_END);
		else if (tok.value == "jmp")
			return Op(tok.loc, OP_JMP);
		else if (tok.value == "cjmpf")
			return Op(tok.loc, OP_CJMPF);
		else if (tok.value == "cjmpt")
			return Op(tok.loc, OP_CJMPT);
		else if (tok.value == "jmpe")
			return Op(tok.loc, OP_JMPE);
		else if (tok.value == "cjmpef")
			return Op(tok.loc, OP_CJMPEF);
		else if (tok.value == "cjmpet")
			return Op(tok.loc, OP_CJMPET);
		// structs
		else if (tok.value == "struct")
			return Op(tok.loc, OP_STRUCT);
		// syscalls
		else if (tok.value == "call0")
			return Op(tok.loc, OP_SYSCALL0);
		else if (tok.value == "call1")
			return Op(tok.loc, OP_SYSCALL1);
		else if (tok.value == "call2")
			return Op(tok.loc, OP_SYSCALL2);
		else if (tok.value == "call3")
			return Op(tok.loc, OP_SYSCALL3);
		else if (tok.value == "call4")
			return Op(tok.loc, OP_SYSCALL4);
		else if (tok.value == "call5")
			return Op(tok.loc, OP_SYSCALL5);
		else if (tok.value == "call6")
			return Op(tok.loc, OP_SYSCALL6);
		// other
		// OP_FUNCTION_CALL
		else if (program.functions.count(tok.value))
			return Op(tok.loc, OP_FUNCTION_CALL, tok.value);
		// OP_LABEL
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
		// OP_SET
		else if (tok.value.front() == '@')
		{
			if (tok.value.size() == 1)
			{
				print_error_at_loc(tok.loc, "unexpected '@' char found while parsing");
				exit(1);
			}
			return Op(tok.loc, OP_SET, tok.value.substr(1));
		}
		// OP_READ
		else if (tok.value.front() == '&')
		{
			if (tok.value.size() == 1)
			{
				print_error_at_loc(tok.loc, "unexpected '&' char found while parsing");
				exit(1);
			}
			return Op(tok.loc, OP_READ, tok.value.substr(1));
		}
		// TODO: remember to support base types in OP_DEFINE_VAR
		// OP_DEFINE_VAR
		else if (program.structs.count(tok.value))
			return Op(tok.loc, OP_DEFINE_VAR, tok.value);
		// OP_PUSH_VAR
		else if (var_offsets.count(tok.value))
			return Op(tok.loc, OP_PUSH_VAR, tok.value);

		else if (tok.value == "[" || tok.value == "]" || tok.value == "(" || tok.value == ")")
		{				
			print_error_at_loc(tok.loc, "unexpected '" + tok.value + "' char found while parsing");
			exit(1);
		}
	}
	else if (tok.type == TOKEN_INT)
		return Op(tok.loc, OP_PUSH_INT, atol(tok.value.c_str()));
	else if (tok.type == TOKEN_STRING)
		return Op(tok.loc, OP_PUSH_STR, add_escapes_to_string(tok.value.substr(1, tok.value.length() - 2)));

	print_error_at_loc(tok.loc, "unknown word '" + tok.value + "'");
	exit(1);
}

std::vector<Op> link_ops(std::vector<Op> ops, std::map<std::string, std::pair<int, int>> labels)
{
	static_assert(OP_COUNT == 52, "unhandled op types in link_ops()");

	for (long unsigned int i = 0; i < ops.size(); i++)
	{
		Op current_op = ops.at(i);

		if (current_op.type == OP_JMP || current_op.type == OP_CJMPF || current_op.type == OP_CJMPT || current_op.type == OP_JMPE || current_op.type == OP_CJMPET || current_op.type == OP_CJMPEF)
		{
			if (labels.count(current_op.str_operand))
			{
				if (current_op.type == OP_JMP || current_op.type == OP_CJMPF || current_op.type == OP_CJMPT)
					current_op.int_operand = labels.at(current_op.str_operand).first;
				else if (current_op.type == OP_JMPE || current_op.type == OP_CJMPET || current_op.type == OP_CJMPEF)
					current_op.int_operand = labels.at(current_op.str_operand).second;
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
	static_assert(OP_COUNT == 52, "unhandled op types in parse_tokens()");

	Program program;
	long unsigned int i = 0;
	int function_addr = 0;

	while (i < tokens.size())
	{
	Op current_op = convert_token_to_op(tokens.at(i), program);
		
		if (current_op.type == OP_FUN)
		{
			i++;
			// check if function name is in the token stream
			if (i >= tokens.size())
			{
				print_error_at_loc(current_op.loc, "unexpected EOF found while parsing function definition");
				exit(1);
			}
		
			// check function name
			Token name_token = tokens.at(i);
			std::string function_name = name_token.value;

			if (!is_legal_name(name_token))
			{
				print_error_at_loc(name_token.loc, "illegal name for function");
				exit(1);
			}
			else if (program.functions.count(function_name))
			{
				print_error_at_loc(name_token.loc, "function '" + function_name + "' already exists");
				exit(1);
			}
			i++;

			// check if eof before argument parsing
			if (i >= tokens.size())
			{
				print_error_at_loc(current_op.loc, "unexpected EOF found while parsing function definition");
				exit(1);
			}

			if (tokens.at(i).value != "(")
			{
				print_error_at_loc(tokens.at(i).loc, "unexpected '" + tokens.at(i).value + "' found while parsing function definition");
				exit(1);
			}
			i++;

			std::vector<LCPType> arg_stack;
			std::vector<LCPType> ret_stack;

			static_assert(BASE_TYPES_COUNT == 2, "unhandled base types in get_struct_member()");
			// parse arguments of function
			while (tokens.at(i).value != ")")
			{
				Token tok = tokens.at(i);

				std::string tok_base_value = parse_type_str(tok.value).first;
				if (tok_base_value != get_base_type_name(TYPE_I64) && tok_base_value != get_base_type_name(TYPE_I8) && !program.structs.count(tok_base_value))
				{
					print_error_at_loc(tok.loc, "unknown argument type '" + tok.value + "'");
					exit(1);
				}
				arg_stack.push_back(LCPType(tok.loc, tok.value));

				i++;
				if (i > tokens.size() - 1)
				{
					print_error_at_loc(tok.loc, "unexpected EOF found while parsing function definition");
					exit(1);
				}
			}

			// check if there is code after function declaration
			i++;
			if (i >= tokens.size())
			{
				print_error_at_loc(tokens.back().loc, "unexpected EOF found while parsing function arguments");
				exit(1);
			}
			
			if (tokens.at(i).value == "[")
			{
				i++;
				// parse return stack of function
				while (tokens.at(i).value != "]")
				{
					Token tok = tokens.at(i);

					std::string tok_base_value = parse_type_str(tok.value).first;
					if (tok_base_value != get_base_type_name(TYPE_I64) && tok_base_value != get_base_type_name(TYPE_I8) && !program.structs.count(tok_base_value))
					{
						print_error_at_loc(tok.loc, "unknown return type '" + tok.value + "'");
						exit(1);
					}
					ret_stack.push_back(LCPType(tok.loc, tok.value));

					i++;
					if (i > tokens.size() - 1)
					{
						print_error_at_loc(tok.loc, "unexpected EOF found while parsing function return stack");
						exit(1);
					}
				}
				i++;
			}

			program.functions.insert({function_name, Function(
				name_token.loc, function_addr, arg_stack, ret_stack
			)});
			function_addr++;

			std::vector<Op> function_ops;
			std::map<std::string, std::pair<int, int>> labels;
			std::map<std::string, std::pair<LCPType, int>> var_offsets;
			int offset = 0;
			bool found_function_end = false;
			// recursion
			int recursion_level = 0;
			std::vector<std::string> recursion_stack;
		
			// parse tokens in function
			while (i < tokens.size())
			{
				Op f_op = convert_token_to_op(tokens.at(i), program, var_offsets);
				
				if (f_op.type == OP_FUN)
				{
					print_error_at_loc(f_op.loc, "unexpected 'fun' keyword found while parsing. functions cannot be defined inside other functions");
					exit(1);
				}
				else if (f_op.type == OP_STRUCT)
				{
					print_error_at_loc(f_op.loc, "unexpected 'struct' keyword found while parsing. Structs cannot be defined inside functions");
					exit(1);
				}
				else if (f_op.type == OP_END)
				{
					if (recursion_level == 0)
					{
						found_function_end = true;
						break;
					}
					else
					{
						recursion_level--;
						std::string ended_label = recursion_stack.back();
						recursion_stack.pop_back();

						// set second idx to end of label	
						std::pair<int, int> label_pair = labels.at(ended_label);
						label_pair.second = i;
						labels.at(ended_label) = label_pair;

						f_op.type = OP_LABEL_END;
						f_op.int_operand = label_pair.second;
						f_op.str_operand = ended_label;
						function_ops.push_back(f_op);	
					}
				}
				else if (f_op.type == OP_LABEL)
				{
					if (labels.count(f_op.str_operand))
					{
						print_error_at_loc(f_op.loc, "redefinition of label '" + f_op.str_operand + "'");
						exit(1);
					}
					std::string label_name = f_op.str_operand;
					std::pair<int, int> start_and_end_idxs;
					start_and_end_idxs.first = i;
					labels.insert({f_op.str_operand, start_and_end_idxs});

					// increase recursion
					recursion_level++;
					recursion_stack.push_back(f_op.str_operand);

					f_op.int_operand = i;
					function_ops.push_back(f_op);
				}
				else if (f_op.type == OP_DEFINE_VAR)
				{
					i++;
					if (i >= tokens.size()) break;
					Token var_name_tok = tokens.at(i);
					if (!is_legal_name(var_name_tok))
					{
						print_error_at_loc(var_name_tok.loc, "illegal name for variable");
						exit(1);
					}
					else if (program.functions.count(var_name_tok.value))
					{
						print_error_at_loc(var_name_tok.loc, "name '" + var_name_tok.value + "' already exists as a function");
						exit(1);
					}
					else if (program.structs.count(var_name_tok.value))
					{
						print_error_at_loc(var_name_tok.loc, "name '" + var_name_tok.value + "' already exists as a struct");
						exit(1);
					}
					else if (var_offsets.count(var_name_tok.value))
					{
						print_error_at_loc(var_name_tok.loc, "variable '" + var_name_tok.value + "' already exists");
						exit(1);
					}
					// TODO: support base types
					var_offsets.insert({var_name_tok.value,
						{LCPType(var_name_tok.loc, f_op.str_operand), offset}
					});
					offset += program.structs.at(f_op.str_operand).size;
					
					// set str_operand to name of variable
					f_op.str_operand = var_name_tok.value;
					function_ops.push_back(f_op);
				}
				else if (f_op.type == OP_SET)
				{
					// if setting a struct
					if (var_offsets.count(f_op.str_operand))
					{
						f_op.type = OP_SET_VAR;
						f_op.int_operand = var_offsets.at(f_op.str_operand).second;
						f_op.int_operand_2 = program.structs.at(var_offsets.at(f_op.str_operand).first.base_type).size; // size of struct
						function_ops.push_back(f_op);
					}
					// if its a struct member
					else
					{
						std::pair<LCPType, int> member_type_offset = get_variable_type_offset(f_op, var_offsets, program.structs);
						// if plain i8
						if (member_type_offset.first.base_type == get_base_type_name(TYPE_I8) && member_type_offset.first.ptr_to_trace == 0)
							f_op.type = OP_SET_VAR_MEMBER_8BIT;
						// if i64 or pointer
						else if (member_type_offset.first.base_type == get_base_type_name(TYPE_I64) || member_type_offset.first.ptr_to_trace > 0)
							f_op.type = OP_SET_VAR_MEMBER_64BIT;
						// struct
						else
						{
							f_op.int_operand_2 = program.structs.at(member_type_offset.first.base_type).size;
							f_op.type = OP_SET_VAR_MEMBER_STRUCT;
						}
						f_op.int_operand = member_type_offset.second; // offset to where member is located
						function_ops.push_back(f_op);
					}
				}
				else if (f_op.type == OP_READ)
				{
					if (var_offsets.count(f_op.str_operand))
					{
						f_op.type = OP_READ_VAR;
						f_op.int_operand = var_offsets.at(f_op.str_operand).second;
						function_ops.push_back(f_op);
					}
					else
					{
						std::pair<LCPType, int> member_type_offset = get_variable_type_offset(f_op, var_offsets, program.structs);
						if (member_type_offset.first.base_type == get_base_type_name(TYPE_I8) && member_type_offset.first.ptr_to_trace == 0)
							f_op.type = OP_READ_VAR_MEMBER_8BIT;
						// if i64 or pointer
						else if (member_type_offset.first.base_type == get_base_type_name(TYPE_I64) || member_type_offset.first.ptr_to_trace > 0)
							f_op.type = OP_READ_VAR_MEMBER_64BIT;
						// struct
						else f_op.type = OP_READ_VAR_MEMBER_STRUCT;

						f_op.int_operand = member_type_offset.second; // offset to where member is located
						function_ops.push_back(f_op);
					}
				}
				else if (f_op.type == OP_JMP || f_op.type == OP_CJMPT || f_op.type == OP_CJMPF || f_op.type == OP_JMPE || f_op.type == OP_CJMPET || f_op.type == OP_CJMPEF)
				{
					i++;
					if (i >= tokens.size()) break;
					Token label_jumpto_tok = tokens.at(i);
					f_op.str_operand = label_jumpto_tok.value;
					function_ops.push_back(f_op);
				}
				else if (f_op.type == OP_PUSH_VAR)
				{
					f_op.int_operand = var_offsets.at(f_op.str_operand).second;
					function_ops.push_back(f_op);
				}
				else function_ops.push_back(f_op);

				i++;
			}
			if (found_function_end)
			{
				program.functions.at(function_name).ops = link_ops(function_ops, labels);
				program.functions.at(function_name).var_offsets = var_offsets;
				program.functions.at(function_name).memory_capacity = offset;
			}
			else
			{
				print_error_at_loc(tokens.back().loc, "Unexpected EOF while parsing function body");
				exit(1);
			}
		}
		else if (current_op.type == OP_STRUCT)
		{
			i++;
			// check if struct name is in the tokens stream
			if (i >= tokens.size())
			{
				print_error_at_loc(current_op.loc, "unexpected EOF found while parsing struct definition");
				exit(1);
			}
		
			// check struct name
			Token name_token = tokens.at(i);
			std::string struct_name = name_token.value;

			if (!is_legal_name(name_token))
			{
				print_error_at_loc(name_token.loc, "illegal name for struct");
				exit(1);
			}
			else if (program.functions.count(struct_name))
			{
				print_error_at_loc(name_token.loc, "name '" + struct_name + "' already exists as a function");
				exit(1);
			}
			else if (program.structs.count(struct_name))
			{
				print_error_at_loc(name_token.loc, "struct '" + struct_name + "' already exists");
				exit(1);
			}
			i++;

			// check if eof before parsing members of struct
			if (i >= tokens.size())
			{
				print_error_at_loc(current_op.loc, "unexpected EOF found while parsing struct definition");
				exit(1);
			}

			static_assert(BASE_TYPES_COUNT == 2, "unhandled base types in parse_tokens()");

			std::map<std::string, std::pair<LCPType, int>> members;
			int offset = 0;
			while (tokens.at(i).value != "end")
			{
				// get type of next member
				Token type_tok = tokens.at(i);
				std::pair<std::string, int> type_info = parse_type_str(type_tok.value);
				std::string base_type = type_info.first;
				int pointer_count = type_info.second;

				if (base_type != get_base_type_name(TYPE_I64) && base_type != get_base_type_name(TYPE_I8) && !program.structs.count(base_type))
				{
					print_error_at_loc(type_tok.loc, "unknown type '" + type_tok.value + "' found while parsing struct definition");
					exit(1);
				}
				// get member name
				i++;
				if (i > tokens.size() - 1)
				{
					print_error_at_loc(type_tok.loc, "unexpected EOF found while parsing function definition");
					exit(1);
				}
				Token member_name_tok = tokens.at(i);
				if (!is_legal_name(member_name_tok))
				{
					print_error_at_loc(member_name_tok.loc, "illegal name for struct member");
					exit(1);
				}

				// set variable type and relative offset
				members.insert({member_name_tok.value, {
					LCPType(member_name_tok.loc, type_tok.value), offset
				}});

				// increase offset
				// if the type is a pointer
				if (pointer_count > 0)
					offset += 8;
				// structs (they have different sizes than words)
				else if (program.structs.count(base_type))
					offset += program.structs.at(base_type).size;
				// 64-bit int
				else if (base_type == get_base_type_name(TYPE_I64))
					offset += 8;
				// 8-bit int
				else if (base_type == get_base_type_name(TYPE_I8))
					offset += 1;

				i++;
				if (i > tokens.size() - 1)
				{
					print_error_at_loc(member_name_tok.loc, "unexpected EOF found while parsing function definition");
					exit(1);
				}
			}
			// align the offset into sections of 8
			program.structs.insert({struct_name, 
				Struct(current_op.loc, members, (offset + 7) / 8 * 8)
			});
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
