#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <assert.h>
#include <map>
#include <algorithm>
#include "../include/token.h"
#include "../include/op.h"
#include "../include/program.h"
#include "../include/error.h"
#include "../include/file.h"
#include "../lexer/lexer.h"

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
	uint64_t i = 0;
	int function_addr = 0;
	std::vector<std::string> include_paths;

	bool is_legal_name(Token token_name);
	std::string add_escapes_to_string(std::string str);
	int64_t eval_const_expression(Location definition_loc);

	Op convert_token_to_op(Token tok, std::map<std::string, std::pair<LangType, int>> var_offsets = {});
	std::vector<Op> link_ops(std::vector<Op> ops, std::map<std::string, std::pair<int, int>> labels);
	void parse_function(Op current_op);
	void parse_struct(Op current_op);
	FunctionSignature parse_func_signature();
	void parse_func_body(Function* func);
};
