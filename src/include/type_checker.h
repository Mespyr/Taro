#pragma once

#include "error.h"
#include "op.h"
#include "program.h"
#include "error.h"
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
	unsigned long int idx;
	std::string func_name;
	Function *function;
	std::vector<LangType> type_stack;
	std::map<std::string, LangType> variables;
	std::map<std::string, std::vector<LangType>> label_stack_states;
	std::vector<std::pair<Op, std::vector<LangType>>> jump_op_stack_states;

	void check_op();
	void check_function();
	bool compare_type_stacks(std::vector<LangType> type_stack_1, std::vector<LangType> type_stack_2);
};
