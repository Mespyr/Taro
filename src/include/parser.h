#pragma once

#include <utility>
#include <vector>
#include <assert.h>
#include <map>
#include <algorithm>
#include "token.h"
#include "op.h"
#include "program.h"
#include "error.h"
#include "file.h"
#include "lexer.h"

class Parser {
public:
	Parser(Lexer *l)
		: lexer(l), tokens(l->tokens)
	{}
	Program program;
	void parse();
private:
	Lexer *lexer;
	std::vector<Token> tokens;
	long unsigned int i = 0;
	int function_addr = 0;
	std::vector<std::string> include_paths;

	bool is_legal_name(Token token_name);
	std::string add_escapes_to_string(std::string str);
	long long eval_const_expression(Location definition_loc);

	Op convert_token_to_op(Token tok, std::map<std::string, std::pair<RambutanType, int>> var_offsets = {});
	std::vector<Op> link_ops(std::vector<Op> ops, std::map<std::string, std::pair<int, int>> labels);
	void parse_function(Op current_op);
	std::pair<std::vector<RambutanType>, std::vector<RambutanType>> parse_func_signature();
	Function parse_func_body(Location loc, std::vector<RambutanType> arg_stack, std::vector<RambutanType> ret_stack);
};
