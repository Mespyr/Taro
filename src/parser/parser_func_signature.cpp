#include "../include/parser.h"

std::pair<std::vector<RambutanType>, std::vector<RambutanType>> Parser::parse_func_signature() {
	if (tokens.at(i).value != "(") {
		print_error_at_loc(tokens.at(i).loc, "unexpected '" + tokens.at(i).value + "' found while parsing function definition");
		exit(1);
	}
	i++;

	std::vector<RambutanType> arg_stack;
	std::vector<RambutanType> ret_stack;

	// parse arguments of function
	while (tokens.at(i).value != ")") {
		Token tok = tokens.at(i);
		std::string tok_base_value = parse_type_str(tok.value).first;
		// if not a primitive type or a struct
		if (!is_prim_type(tok_base_value) && !program.structs.count(tok_base_value)) {
			print_error_at_loc(tok.loc, "unknown argument type '" + tok.value + "'");
			exit(1);
		}
		arg_stack.push_back(RambutanType(tok.loc, tok.value));

		i++;
		if (i > tokens.size() - 1) {
			print_error_at_loc(tok.loc, "unexpected EOF found while parsing function definition");
			exit(1);
		}
	}

	// check if there is code after function declaration
	i++;
	if (i >= tokens.size()) {
		print_error_at_loc(tokens.back().loc, "unexpected EOF found while parsing function arguments");
		exit(1);
	}
	
	if (tokens.at(i).value == "[") {
		i++;
		// parse return stack of function
		while (tokens.at(i).value != "]") {
			Token tok = tokens.at(i);
			std::string tok_base_value = parse_type_str(tok.value).first;
			// if isn't a primitive type or struct
			if (!is_prim_type(tok_base_value) && !program.structs.count(tok_base_value)) {
				print_error_at_loc(tok.loc, "unknown return type '" + tok.value + "'");
				exit(1);
			}
			ret_stack.push_back(RambutanType(tok.loc, tok.value));

			i++;
			if (i > tokens.size() - 1) {
				print_error_at_loc(tok.loc, "unexpected EOF found while parsing function return stack");
				exit(1);
			}
		}
		i++;
	}

	return {arg_stack, ret_stack};
}
