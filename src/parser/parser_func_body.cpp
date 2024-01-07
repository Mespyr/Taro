#include "../include/parser.h"

void Parser::parse_func_body(Function* func) {
	std::map<std::string, std::pair<int, int>> labels;
	bool found_function_end = false;
	int recursion_level = 0;
	std::vector<std::string> recursion_stack;

	while (i < tokens.size()) {
		Op f_op = convert_token_to_op(tokens.at(i), func->var_offsets);
		
		if (f_op.type == OP_FUN) {
			print_error_at_loc(f_op.loc, "unexpected 'fun' keyword found inside function definition");
			exit(1);
		}
		else if (f_op.type == OP_STRUCT) {
			print_error_at_loc(f_op.loc, "unexpected 'struct' keyword found inside function definition");
			exit(1);
		}
		else if (f_op.type == OP_END) {
			if (recursion_level == 0) {
				found_function_end = true;
				func->ops = link_ops(func->ops, labels);
				break;
			}
			else {
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
				func->ops.push_back(f_op);
			}
		}
		else if (f_op.type == OP_LABEL) {
			if (labels.count(f_op.str_operand)) {
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
			func->ops.push_back(f_op);
		}
		else if (f_op.type == OP_DEFINE_VAR) {
			i++;
			if (i >= tokens.size()) break;
			Token var_name_tok = tokens.at(i);
			if (!is_legal_name(var_name_tok)) {
				print_error_at_loc(var_name_tok.loc, "illegal name for variable");
				exit(1);
			}
			else if (program.functions.count(var_name_tok.value)) {
				print_error_at_loc(var_name_tok.loc, "name '" + var_name_tok.value + "' already exists as a function");
				exit(1);
			}
			else if (program.structs.count(var_name_tok.value)) {
				print_error_at_loc(var_name_tok.loc, "name '" + var_name_tok.value + "' already exists as a struct");
				exit(1);
			}
			else if (func->var_offsets.count(var_name_tok.value)) {
				print_error_at_loc(var_name_tok.loc, "variable '" + var_name_tok.value + "' already exists");
				exit(1);
			}

			func->var_offsets.insert({var_name_tok.value,
				{LangType(var_name_tok.loc, f_op.str_operand), func->memory_capacity}
			});

			func->memory_capacity += sizeof_type(f_op.str_operand, program.structs);
		}
		else if (f_op.type == OP_SET) {
			// if setting a variable
			if (func->var_offsets.count(f_op.str_operand)) {
				static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in parse_tokens()");
				LangType type = func->var_offsets.at(f_op.str_operand).first;
				if (is_prim_type(type.base_type) || is_pointer(type)) {
					// 8bit values
					if (sizeof_type(type) == 1)
						f_op.mode = MODE_8BIT;
					// 64bit values
					else if (sizeof_type(type) == 8)
						f_op.mode = MODE_64BIT;
				}
				else f_op.mode = MODE_STRUCT;
				f_op.type = OP_SET_VAR;
				f_op.int_operand = func->var_offsets.at(f_op.str_operand).second; // offset to location of variable
				f_op.int_operand_2 = sizeof_type(type, program.structs); // size of type (amount of data needed to move it around)
				func->ops.push_back(f_op);
			}
			else if (program.structs.count(f_op.str_operand) || is_prim_type(f_op.str_operand) || is_pointer(f_op.str_operand)) {
				static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in parse_tokens()");
				// if setting value of pointer
				if (is_pointer(f_op.str_operand)) {
					std::pair<std::string, int> type_pair = parse_type_str(f_op.str_operand);
					if (program.structs.count(type_pair.first) || is_prim_type(type_pair.first))
						f_op.mode = MODE_64BIT;
					else {
						print_error_at_loc(f_op.loc, "unknown type or variable name '" + f_op.str_operand + "'");
						exit(1);
					}
				}
				// if setting primitive type
				else if (is_prim_type(f_op.str_operand)) {
					// 8bit values
					if (sizeof_type(f_op.str_operand) == 1)
						f_op.mode = MODE_8BIT;
					// 64bit values
					else if (sizeof_type(f_op.str_operand) == 8)
						f_op.mode = MODE_64BIT;
				}
				else f_op.mode = MODE_STRUCT;
				f_op.type = OP_SET_PTR;
				// since setting variable relatively via pointers, there is no global offset to provide.
				f_op.int_operand = sizeof_type(f_op.str_operand, program.structs); // size of type (amount of data needed to move around)
				func->ops.push_back(f_op);
			}
			// if setting value of struct member
			else {
				static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in parse_tokens()");
				std::vector<std::string> split_cmd = split_by_dot(f_op.str_operand);

				// if there is only one value in the command ie: String, meaning that a variable String doesn't exist and a type String doesnt exist
				// because we check if it is one of those in the previous if-statements
				if (split_cmd.size() == 1) {
					print_error_at_loc(f_op.loc, "unknown type or variable name '" + split_cmd.front() + "'");
					exit(1);
				}
				
				// if setting pointer member
				if (program.structs.count(split_cmd.front())) {
					std::pair<LangType, int> member_type_offset = struct_member_offset(f_op, program.structs);
					f_op.type = OP_SET_PTR_STRUCT_MEMBER;
					if (member_type_offset.first.ptr_to_trace > 0) // if it is a pointer, whether to a primitive or a struct
						f_op.mode = MODE_64BIT;
					else if (program.structs.count(member_type_offset.first.base_type))
						f_op.mode = MODE_STRUCT;
					// 8bit values
					else if (sizeof_type(member_type_offset.first, program.structs) == 1)
						f_op.mode = MODE_8BIT;
					// 64bit values
					else if (sizeof_type(member_type_offset.first, program.structs) == 8)
						f_op.mode = MODE_64BIT;

					f_op.int_operand = member_type_offset.second; // relative offset of member in pointer
					f_op.int_operand_2 = sizeof_type(member_type_offset.first, program.structs); // size of member
					func->ops.push_back(f_op);
				}
				// if setting variable member
				else {
					std::pair<LangType, int> member_type_offset = variable_member_offset(f_op, func->var_offsets, program.structs);
					if (member_type_offset.first.ptr_to_trace > 0) // if it is a pointer, whether to a primitive or a struct
						f_op.mode = MODE_64BIT;
					else if (program.structs.count(member_type_offset.first.base_type))
						f_op.mode = MODE_STRUCT;
					// 8bit values
					else if (sizeof_type(member_type_offset.first, program.structs) == 1)
						f_op.mode = MODE_8BIT;
					// 64bit values
					else if (sizeof_type(member_type_offset.first, program.structs) == 8)
						f_op.mode = MODE_64BIT;
					f_op.type = OP_SET_VAR_STRUCT_MEMBER;
					f_op.int_operand = member_type_offset.second; // offset to where member is located
					f_op.int_operand_2 = sizeof_type(member_type_offset.first, program.structs); // size of member
					func->ops.push_back(f_op);
				}
			}
		}
		else if (f_op.type == OP_READ) {
			// if reading a variable
			if (func->var_offsets.count(f_op.str_operand)) {
				LangType type = func->var_offsets.at(f_op.str_operand).first;
				if (!is_prim_type(type) && !is_pointer(type)) {
					print_error_at_loc(f_op.loc, "Can't read whole variable for a non-primitive type (type " + human_readable_type(type) + ")");
					exit(1);
				}

				static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in parse_tokens()");
				// 8bit values
				if (sizeof_type(type) == 1)
					f_op.mode = MODE_8BIT;
				// 64bit values
				else if (sizeof_type(type) == 8)
					f_op.mode = MODE_64BIT;

				f_op.type = OP_READ_VAR;
				f_op.int_operand = func->var_offsets.at(f_op.str_operand).second;
				func->ops.push_back(f_op);
			}
			// if reading value of pointer
			else if (program.structs.count(f_op.str_operand) || is_prim_type(f_op.str_operand)) {
				if (!is_prim_type(f_op.str_operand) && !is_pointer(f_op.str_operand)) {
					print_error_at_loc(f_op.loc, "Can't read and push struct onto the stack from a pointer to it (type " + f_op.str_operand + ")");
					exit(1);
				}

				static_assert(MODE_COUNT == 3, "unhandled OpCodeModes in parse_tokens()");
				// 8bit values
				if (sizeof_type(f_op.str_operand) == 1)
					f_op.mode = MODE_8BIT;
				// 64bit values
				else if (sizeof_type(f_op.str_operand) == 8)
					f_op.mode = MODE_64BIT;

				f_op.type = OP_READ_PTR;
				f_op.int_operand = sizeof_type(f_op.str_operand, program.structs);
				func->ops.push_back(f_op);
			}
			// if reading value of struct member
			else {
				static_assert(PRIM_TYPES_COUNT == 2, "unhandled prim types in parse_tokens()");
				std::vector<std::string> split_cmd = split_by_dot(f_op.str_operand);
				
				// if reading pointer member
				if (program.structs.count(split_cmd.front())) {
					std::pair<LangType, int> member_type_offset = struct_member_offset(f_op, program.structs);
					f_op.type = OP_READ_PTR_STRUCT_MEMBER;

					if (member_type_offset.first.ptr_to_trace > 0) // if it is a pointer, whether to a primitive or a struct
						f_op.mode = MODE_64BIT;
					else if (program.structs.count(member_type_offset.first.base_type))
						f_op.mode = MODE_STRUCT;
					// 8bit values
					else if (sizeof_type(member_type_offset.first, program.structs) == 1)
						f_op.mode = MODE_8BIT;
					// 64bit values
					else if (sizeof_type(member_type_offset.first, program.structs) == 8)
						f_op.mode = MODE_64BIT;

					f_op.int_operand = member_type_offset.second; // relative offset of member in pointer
					f_op.int_operand_2 = sizeof_type(member_type_offset.first, program.structs);
					func->ops.push_back(f_op);
				}
				// if reading variable member
				else {
					std::pair<LangType, int> member_type_offset = variable_member_offset(f_op, func->var_offsets, program.structs);
					if (member_type_offset.first.ptr_to_trace > 0) // if it is a pointer, whether to a primitive or a struct
						f_op.mode = MODE_64BIT;
					else if (program.structs.count(member_type_offset.first.base_type))
						f_op.mode = MODE_STRUCT;
					// 8bit values
					else if (sizeof_type(member_type_offset.first, program.structs) == 1)
						f_op.mode = MODE_8BIT;
					// 64bit values
					else if (sizeof_type(member_type_offset.first, program.structs) == 8)
						f_op.mode = MODE_64BIT;

					f_op.type = OP_READ_VAR_STRUCT_MEMBER;
					f_op.int_operand = member_type_offset.second; // offset to where member is located
					f_op.int_operand_2 = sizeof_type(member_type_offset.first, program.structs);
					func->ops.push_back(f_op);
				}
			}
		}
		else if (f_op.type == OP_JMP || f_op.type == OP_CJMPT || f_op.type == OP_CJMPF || f_op.type == OP_JMPE || f_op.type == OP_CJMPET || f_op.type == OP_CJMPEF) {
			i++;
			if (i >= tokens.size()) break;
			Token label_jumpto_tok = tokens.at(i);
			f_op.str_operand = label_jumpto_tok.value;
			func->ops.push_back(f_op);
		}
		else if (f_op.type == OP_PUSH_VAR) {
			f_op.int_operand = func->var_offsets.at(f_op.str_operand).second;
			func->ops.push_back(f_op);
		}
		else if (f_op.type == OP_PUSH_TYPE_INSTANCE) {
			f_op.int_operand = sizeof_type(f_op.str_operand, program.structs);
			func->ops.push_back(f_op);
		}
		else func->ops.push_back(f_op);
		i++;
	}
	if (!found_function_end) {
		print_error_at_loc(tokens.back().loc, "Unexpected EOF while parsing function body");
		exit(1);
	}
};
