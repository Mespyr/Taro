#include "parser.h"

std::vector<Op> Parser::link_ops(std::vector<Op> ops, std::map<std::string, std::pair<int, int>> labels) {
	for (uint64_t i = 0; i < ops.size(); i++) {
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
	while (i < tokens.size()) {
		Op current_op = convert_token_to_op(tokens.at(i));

		switch (current_op.type) {

		case OP_FUN:
			parse_function(current_op);
			break;

		case OP_STRUCT:
			parse_struct(current_op);
			break;

		case OP_IMPORT: {
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
		} break;

		case OP_CONST: {
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

			int64_t eval = eval_const_expression(name_token.loc);
			program.consts.insert({const_name, Const(name_token.loc, eval)});
		} break;

		default:
			print_error_at_loc(current_op.loc, "Unexpected token found while parsing");
			exit(1);
			break;
		}

		i++;
	}
}
