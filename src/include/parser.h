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
	Program program;
	void parse();
private:
	Lexer *lexer;
	std::vector<Token> tokens;
	long unsigned int i = 0;

	bool is_legal_name(Token token_name);
	std::string add_escapes_to_string(std::string str);
	long long eval_const_expression(Location definition_loc);

	Op convert_token_to_op(Token tok, std::map<std::string, std::pair<RambutanType, int>> var_offsets = {});
	std::vector<Op> link_ops(std::vector<Op> ops, std::map<std::string, std::pair<int, int>> labels);
};
