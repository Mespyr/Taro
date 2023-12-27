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
	bool compare_type_stacks(std::vector<RambutanType> type_stack_1, std::vector<RambutanType> type_stack_2);
};
