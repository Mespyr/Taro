#include "include/op.h"

bool is_builtin_word(std::string word)
{
	static_assert(OP_COUNT == 9, "unhandled op types in type_check_program()");
	std::vector<std::string> builtin_words = {
		"dump",
		"+", "-", "*", "/",
		"sec", "end"
	};

	for (std::string w : builtin_words)
		if (w == word)
			return true;

	return false;
};
