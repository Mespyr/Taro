#include "../include/parser.h"

void Parser::parse_function(Op current_op) {
	i++;
	// check if function name is in the token stream
	if (i >= tokens.size()) {
		print_error_at_loc(current_op.loc, "unexpected EOF found while parsing function definition");
		exit(1);
	}

	// check function name
	Token name_token = tokens.at(i);
	std::string function_name = name_token.value;

	if (!is_legal_name(name_token)) {
		print_error_at_loc(name_token.loc, "illegal name for function");
		exit(1);
	}
	else if (program.functions.count(function_name)) {
		print_error_at_loc(name_token.loc, "function '" + function_name + "' already exists");
		exit(1);
	}
	else if (program.structs.count(function_name)) {
		print_error_at_loc(name_token.loc, "struct '" + function_name + "' already exists");
		exit(1);
	}
	i++;

	// check if eof before argument parsing
	if (i >= tokens.size()) {
		print_error_at_loc(current_op.loc, "unexpected EOF found while parsing function definition");
		exit(1);
	}

	FunctionSignature signature = parse_func_signature();

	Function func = Function(name_token.loc, function_addr, signature);
	program.functions.insert({function_name, func});
	parse_func_body(&func);
	program.functions.at(function_name) = func;
	function_addr++;
}
