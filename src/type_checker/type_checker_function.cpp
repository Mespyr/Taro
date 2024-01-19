#include "type_checker.h"

void TypeChecker::check_function() {
	// reset everything
	type_stack.clear();
	variables.clear();
	label_stack_states.clear();
	jump_op_stack_states.clear();
	idx = 0;

	for (LangType t : function->signature.argument_stack)
		type_stack.push_back(t);

	while (idx < function->ops.size()) {
		check_op();
		idx++;
	}
	
	// make sure return values match up with what is specified in the function definition and throw error if not
	if (!compare_type_stacks(type_stack, function->signature.return_stack)) {
		// main cannot have excess data on stack
		if (function->signature.return_stack.size() < type_stack.size()) {
			print_error_at_loc(function->loc, "unhandled data on the stack (expected " + std::to_string(function->signature.return_stack.size()) + " items, got " + std::to_string(type_stack.size()) + ")");

			std::vector<LangType> excess_stack;
			for (uint64_t i = type_stack.size() - function->signature.return_stack.size(); i > 0; i--) {
				excess_stack.push_back(type_stack.back());
				type_stack.pop_back();
			}
			std::reverse(excess_stack.begin(), excess_stack.end());

			for (LangType t : excess_stack)
			print_note_at_loc(t.loc, "excess data pushed here (" + human_readable_type(t) + ")");
		}

		else if (function->signature.return_stack.size() > type_stack.size())
			print_error_at_loc(function->loc, "not enough data on the stack (expected " + std::to_string(function->signature.return_stack.size()) + " items, got " + std::to_string(type_stack.size()) + ")");

		else if (type_stack.size() > 1) {
			print_invalid_combination_of_types_error(function->loc, type_stack, func_name, "", true);
			for (LangType t : type_stack)
				print_note_at_loc(t.loc, "argument pushed here (" + human_readable_type(t) + ")");
		}

		else {
			print_invalid_type_error(function->loc, human_readable_type(function->signature.return_stack.at(0)), human_readable_type(type_stack.at(0)), func_name, "", true);
			print_note_at_loc(type_stack.at(0).loc, "value pushed here (" + human_readable_type(type_stack.at(0)) + ")");
		}

		exit(1);
	}

	// go through code again and check jmp labels to see if the stack values are the same as the label they are jumping to
	for (std::pair<Op, std::vector<LangType>> op_stack_pair : jump_op_stack_states) {
		Op op = op_stack_pair.first;
		std::vector<LangType> op_stack_state = op_stack_pair.second;

		if (!label_stack_states.count(op.str_operand)) {
			print_error_at_loc(op.loc, "error found in parser or type-checker. Label '" + op.str_operand + " either doesn't exist or hasn't been found by the parser.");
			exit(1);
		}

		if (!compare_type_stacks(op_stack_state, label_stack_states.at(op.str_operand))) {
			print_error_at_loc(op.loc, "different types on stack then what was expected at the label.");
			exit(1);
		}
	}
}
