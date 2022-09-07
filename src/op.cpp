#include "include/op.h"

bool is_builtin_word(std::string word)
{
	static_assert(OP_COUNT == 39, "unhandled op types in type_check_program()");
	std::vector<std::string> builtin_words = {
		"dump",
		"+", "-", "*", "/",
		"=", ">", "<", ">=", "<=", "!=", "not", "and", "or",
		"pop", "dup", "swp", "rot", "over",
		"call0", "call1", "call2", "call3", "call4", "call5", "call6",
		"fun", "end", "jmp", "cjmpt", "cjmpf", "jmpe", "cjmpet", "cjmpef"
	};

	for (std::string w : builtin_words)
		if (w == word)
			return true;

	return false;
};
