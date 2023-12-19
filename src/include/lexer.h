#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <assert.h>
#include "token.h"
#include "error.h"
#include "file.h"

class Lexer {
public:
	std::vector<Token> tokens;

	void set_file(std::string filename);
	void tokenize();
private:
	std::string filename;
	std::string line;
	long unsigned int line_number = 0;

	bool is_number(std::string n);
	long unsigned int find_next_token_start_col(long unsigned int column_number);
	long unsigned int find_token_end_col(long unsigned int column_number);
	long unsigned int find_string_end_col(long unsigned int column_number);
	void tokenize_line();
};
