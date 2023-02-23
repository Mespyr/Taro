#pragma once

#include "error.h"
#include "program.h"
#include "parser.h"

struct ConstExpr {
	long long value;
	long unsigned int i;
};

ConstExpr eval_const_expression(Program program, std::vector<Token> tokens, long unsigned int i, Location definition_loc);
