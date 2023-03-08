#pragma once

#include "error.h"
#include "op.h"
#include "types.h"
#include "program.h"
#include "error.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>

void verify_program(Program program);
bool compare_type_stacks(std::vector<RambutanType> type_stack_1, std::vector<RambutanType> type_stack_2);
void type_check_program(Program &program);
