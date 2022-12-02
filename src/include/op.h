#ifndef _LCP_OP_H_
#define _LCP_OP_H_

#include <string>
#include <vector>
#include <assert.h>
#include "location.h"
#include "token.h"

enum OpCodeMode
{
	MODE_8BIT,
	MODE_64BIT,
	MODE_STRUCT,

	MODE_COUNT
};

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
	OP_STRUCT,
	// variables
	OP_DEFINE_VAR,
	OP_SET,
	OP_READ,

	OP_SET_VAR,
	OP_SET_VAR_FROM_OTHER_PTR,
	OP_SET_VAR_STRUCT_MEMBER,
	OP_READ_VAR,
	OP_READ_VAR_STRUCT_MEMBER,
	OP_PUSH_VAR,

	OP_SET_PTR,
	OP_SET_PTR_FROM_OTHER_PTR,
	OP_SET_PTR_STRUCT_MEMBER,
	OP_READ_PTR,
	OP_READ_PTR_STRUCT_MEMBER,

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

	bool is_prim_type_mode();

	Location loc;
	OpType type;

	long long int_operand;
	long long int_operand_2;
	std::string str_operand;
	OpCodeMode mode;
};

bool is_builtin_word(std::string word);

#endif
