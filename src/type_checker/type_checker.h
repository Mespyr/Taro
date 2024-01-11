#pragma once

#include "../include/op.h"
#include "../include/program.h"
#include "../include/error.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>

class TypeChecker {
public:
	TypeChecker(Program program)
		: program(program)
	{}
	Program program;

	void verify();
	void perform_checks();
private:
	std::string func_name;
	Function *function;
	unsigned long int idx;
	std::vector<LangType> type_stack;
	std::map<std::string, LangType> variables;
	std::map<std::string, std::vector<LangType>> label_stack_states;
	std::vector<std::pair<Op, std::vector<LangType>>> jump_op_stack_states;

	void handle_arithmetic_op(Op op);
	void handle_comparison_op(Op op);
	void handle_syscall_op(Op op);
	void handle_jump_op(Op op);
	void check_op();
	void check_function();
	bool compare_type_stacks(std::vector<LangType> type_stack_1, std::vector<LangType> type_stack_2);
};
