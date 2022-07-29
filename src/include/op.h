#ifndef _LCP_OP_H_
#define _LCP_OP_H_

#include "location.h"
#include "token.h"
#include <string>
#include <vector>

enum OpType
{
	// debugging
	OP_DUMP,
	// arithmetics
	OP_PLUS,
	OP_MINUS,
	OP_MUL,
	OP_DIV,
	// keywords
	OP_SEC,
	OP_END,
	// other
	OP_PUSH_INT,
	OP_FUNCTION_CALL,
	// count | must always be last
	OP_COUNT
};

class Op
{
public:
	Op( Location loc, OpType type) : 
		loc(loc),
		type(type)
	{}

	Op(Location loc, OpType type, long long int_operand) :
		loc(loc),
		type(type),
		int_operand(int_operand)
	{}

	Op(Location loc, OpType type, std::string str_operand) :
		loc(loc),
		type(type),
		str_operand(str_operand)
	{}

	Location loc;
	OpType type;

	long long int_operand;
	std::string str_operand;
};

bool is_builtin_word(std::string word);

#endif
