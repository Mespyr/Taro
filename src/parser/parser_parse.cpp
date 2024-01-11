#include "parser.h"

std::vector<Op> Parser::link_ops(std::vector<Op> ops, std::map<std::string, std::pair<int, int>> labels) {
	static_assert(OP_COUNT == 57, "unhandled op types in Parser::link_ops()");

	for (long unsigned int i = 0; i < ops.size(); i++) {
		Op current_op = ops.at(i);

		if (current_op.type == OP_JMP || current_op.type == OP_CJMPF || current_op.type == OP_CJMPT || current_op.type == OP_JMPE || current_op.type == OP_CJMPET || current_op.type == OP_CJMPEF) {
			if (labels.count(current_op.str_operand)) {
				if (current_op.type == OP_JMP || current_op.type == OP_CJMPF || current_op.type == OP_CJMPT)
					current_op.int_operand = labels.at(current_op.str_operand).first;
				else if (current_op.type == OP_JMPE || current_op.type == OP_CJMPET || current_op.type == OP_CJMPEF)
					current_op.int_operand = labels.at(current_op.str_operand).second;
				ops.at(i) = current_op;
			}
			else {
				print_error_at_loc(current_op.loc, "label '" + current_op.str_operand + "' not found");
				exit(1);
			}
		}
	}

	return ops;
}

void Parser::parse() {
	static_assert(OP_COUNT == 57, "unhandled op types in Parser::parse()");

	while (i < tokens.size()) {
		Op current_op = convert_token_to_op(tokens.at(i));
		
		if (current_op.type == OP_FUN)
			parse_function(current_op);
		else if (current_op.type == OP_STRUCT) {
			i++;
			// check if struct name is in the tokens stream
			if (i >= tokens.size()) {
				print_error_at_loc(current_op.loc, "unexpected EOF found while parsing struct definition");
				exit(1);
			}
		
			// check struct name
			Token name_token = tokens.at(i);
			std::string struct_name = name_token.value;

			if (!is_legal_name(name_token)) {
				print_error_at_loc(name_token.loc, "illegal name for struct");
				exit(1);
			}
			else if (program.functions.count(struct_name)) {
				print_error_at_loc(name_token.loc, "name '" + struct_name + "' already exists as a function");
				exit(1);
			}
			else if (program.structs.count(struct_name)) {
				print_error_at_loc(name_token.loc, "struct '" + struct_name + "' already exists");
				exit(1);
			}
			i++;

			// check if eof before parsing members of struct
			if (i >= tokens.size()) {
				print_error_at_loc(current_op.loc, "unexpected EOF found while parsing struct definition");
				exit(1);
			}

			std::map<std::string, std::pair<LangType, int>> members;
			int offset = 0;
			while (tokens.at(i).value != "end") {
				// get type of next member
				Token type_tok = tokens.at(i);
				std::pair<std::string, int> type_info = parse_type_str(type_tok.value);
				std::string base_type = type_info.first;
				int pointer_count = type_info.second;
			
				// if type is an existing type (primitive or struct)
				// or if the type is a pointer to the currently defined type
				if (!is_prim_type(base_type) && !program.structs.count(base_type)) {
					if (base_type == struct_name && pointer_count > 0);
					else {
						print_error_at_loc(type_tok.loc, "unknown type '" + type_tok.value + "' found while parsing struct definition");
						exit(1);
					}
				}
				// get member name
				i++;
				if (i > tokens.size() - 1) {
					print_error_at_loc(type_tok.loc, "unexpected EOF found while parsing function definition");
					exit(1);
				}
				Token member_name_tok = tokens.at(i);
				if (!is_legal_name(member_name_tok)) {
					print_error_at_loc(member_name_tok.loc, "illegal name for struct member");
					exit(1);
				}

				// set variable type and relative offset
				members.insert({member_name_tok.value, {
					LangType(member_name_tok.loc, type_tok.value), offset
				}});

				// increase offset by size of type
				if (pointer_count > 0) offset += 8;
				else offset += sizeof_type(base_type, program.structs);

				i++;
				if (i > tokens.size() - 1) {
					print_error_at_loc(member_name_tok.loc, "unexpected EOF found while parsing function definition");
					exit(1);
				}
			}
			// align the offset into sections of 8
			program.structs.insert({struct_name, 
				Struct(current_op.loc, members, (offset + 7) / 8 * 8)
			});
		}
		else if (current_op.type == OP_IMPORT) {
			if (i++ == tokens.size()) {
				print_error_at_loc(current_op.loc, "Unexpected 'import' keyword found while parsing");
				exit(1);
			}

			Token include_file_token = tokens.at(i);
			if (include_file_token.type != TOKEN_STRING) {
				print_error_at_loc(include_file_token.loc, "Was expecting token of type string after 'import' statement");
				exit(1);
			}

			std::string file_path = include_file_token.value.substr(1,include_file_token.value.length() - 2);
			File file(file_path, FILE_READ);
			if (!file.exists()) {
				print_error_at_loc(include_file_token.loc, "No such file or directory, '" + file_path + "'");
				exit(1);
			}

			if (std::find(include_paths.begin(), include_paths.end(), file_path) == include_paths.end())
			{
				lexer->set_file(file_path);
				lexer->tokenize();
				tokens.insert(tokens.begin() + i + 1, lexer->tokens.begin(), lexer->tokens.end());
				include_paths.push_back(file_path);
			}
		}
		else if (current_op.type == OP_CONST) {
			i++;
			// check if function name is in the token stream
			if (i >= tokens.size()) {
				print_error_at_loc(current_op.loc, "unexpected EOF found while parsing function definition");
				exit(1);
			}
		
			// check function name
			Token name_token = tokens.at(i);
			std::string const_name = name_token.value;

			if (!is_legal_name(name_token)) {
				print_error_at_loc(name_token.loc, "illegal name for function");
				exit(1);
			}
			else if (program.functions.count(const_name)) {
				print_error_at_loc(name_token.loc, "function '" + const_name + "' already exists");
				exit(1);
			}
			else if (program.structs.count(const_name)) {
				print_error_at_loc(name_token.loc, "struct '" + const_name + "' already exists");
				exit(1);
			}
			i++;

			// check if eof before parsing expression
			if (i >= tokens.size()) {
				print_error_at_loc(current_op.loc, "unexpected EOF found while parsing function definition");
				exit(1);
			}

			long long eval = eval_const_expression(name_token.loc);
			program.consts.insert({const_name, Const(name_token.loc, eval)});
		}
		else {
			print_error_at_loc(current_op.loc, "Unexpected token found while parsing");
			exit(1);
		}

		i++;
	}
}
