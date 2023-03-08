#pragma once

#include <vector>
#include <assert.h>
#include <map>
#include "token.h"
#include "op.h"
#include "program.h"
#include "error.h"
#include "types.h"

bool is_legal_name(Token token_name);
std::string add_escapes_to_string(std::string str);
Op convert_token_to_op(Token tok, Program program, std::map<std::string, std::pair<RambutanType, int>> var_offsets = {});
std::vector<Op> link_ops(std::vector<Op> ops, std::map<std::string, std::pair<int, int>> labels);
Program parse_tokens(std::vector<Token> tokens);
