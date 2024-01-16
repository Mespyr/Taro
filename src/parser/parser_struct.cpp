#include "parser.h"

void Parser::parse_struct(Op current_op) {
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

	if (tokens.at(i).value != "(") {
		print_error_at_loc(tokens.at(i).loc, "unexpected token found while parsing");
		exit(1);
	}
	i++;

	std::map<std::string, std::pair<LangType, int>> members;
	int offset = 0;
	while (tokens.at(i).value != ")") {
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
