#include "type_checker.h"

void TypeChecker::perform_checks() {
	for (auto fn_key = program.functions.begin(); fn_key != program.functions.end(); fn_key++) {
		func_name = fn_key->first;
		function = &fn_key->second;
		check_function();
	}
}
