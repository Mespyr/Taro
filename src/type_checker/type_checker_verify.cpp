#include "type_checker.h"

void TypeChecker::verify() {
	if (!program.functions.count("main")) {
		print_error("no entry point found in program (no 'main' function)");
		exit(1);
	}

	Function main_func = program.functions.at("main");
	if (main_func.signature.argument_stack.size() > 0) {
		print_error_at_loc(main_func.loc, "'main' function must not pass any arguments");
		exit(1);
	}

	if (main_func.signature.return_stack.size() > 0) {
		print_error_at_loc(main_func.loc, "'main' function must not have any return values");
		exit(1);
	}
}
