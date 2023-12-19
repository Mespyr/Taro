#pragma once

#include <vector>
#include <assert.h>
#include <map>
#include <algorithm>
#include "token.h"
#include "op.h"
#include "program.h"
#include "error.h"
#include "types.h"
#include "file.h"
#include "lexer.h"

class Parser {
public:
	Parser(Lexer *l);

	std::vector<Token> tokens;
	Program program;

	Op convert_token_to_op(Token tok, std::map<std::string, std::pair<RambutanType, int>> var_offsets = {});
	void parse();
private:
	Lexer *lexer;

	bool is_legal_name(Token token_name);
	std::string add_escapes_to_string(std::string str);
	std::vector<Op> link_ops(std::vector<Op> ops, std::map<std::string, std::pair<int, int>> labels);
};

#include "eval.h"
