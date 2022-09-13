#ifndef _LCP_OP_H_
#define _LCP_OP_H_

#include <string>
#include <vector>
#include <assert.h>
#include "location.h"
#include "token.h"

enum OpType
{
	// debugging
	OP_DUMP,
	// arithmetics
	OP_PLUS,
	OP_MINUS,
	OP_MUL,
	OP_DIV,
	// comparisons
	OP_EQUAL,
	OP_GREATER,
	OP_LESS,
	OP_GREATER_EQ,
	OP_LESS_EQ,
	OP_NOT_EQ,
	OP_NOT,
	OP_AND,
	OP_OR,
	// stack manipulation
	OP_POP,
	OP_DUP,  // [a] -> [a, a]
	OP_SWP,  // [a, b] -> [b, a]
	OP_ROT,  // [a, b, c] -> [b, c, a]
	OP_OVER, // [a, b] -> [a, b a]
	// keywords
	OP_FUN,
	OP_END,
	// syscalls
	OP_SYSCALL0,
	OP_SYSCALL1,
	OP_SYSCALL2,
	OP_SYSCALL3,
	OP_SYSCALL4,
	OP_SYSCALL5,
	OP_SYSCALL6,
	// labels
	OP_LABEL,
	OP_LABEL_END,
	OP_JMP,
	OP_CJMPT,
	OP_CJMPF,
	OP_JMPE,
	OP_CJMPET,
	OP_CJMPEF,
	// other
	OP_PUSH_INT,
	OP_PUSH_STR,
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
