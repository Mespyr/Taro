#include "../include/lexer.h"

void Lexer::tokenize_line() {
	static_assert(TOKEN_TYPE_COUNT == 3, "unhandled token types in tokenize_line()");

	// get starting position of first token
	long unsigned int column_number_start = find_next_token_start_col(0);
	long unsigned int column_number_end;

	while (column_number_start < line.length()) {
		// if there is a comment, then ignore rest of line
		if (line.at(column_number_start) == '#')
			return;

		// if token is a string
		if (line.at(column_number_start) == '"') {
			// get end position of string
			column_number_end = find_string_end_col(column_number_start);

			// check if col_end is at end of line
			if (column_number_end == line.length()) {
				print_error_at_loc(
					Location(line_number, column_number_start, column_number_end, line, filename),
					"unexpected EOL while tokenizing string"
				);
				exit(1);
			}
			// inc col_end so it points to after '"'
			column_number_end++;

			tokens.push_back(Token(
				line.substr(column_number_start, column_number_end - column_number_start), TOKEN_STRING,
				Location(line_number, column_number_start, column_number_end, line, filename)
			));
		}
		// if token is any of these chars ()[]
		else if (line.at(column_number_start) == '(' || line.at(column_number_start) == ')' || line.at(column_number_start) == '[' || line.at(column_number_start) == ']') {
			tokens.push_back(Token(
				line.substr(column_number_start, 1), TOKEN_WORD,
				Location(line_number, column_number_start, column_number_start+1, line, filename)
			));
			// one after the char
			column_number_end = column_number_start + 1;
		}
		else {
			column_number_end = find_token_end_col(column_number_start);

			std::string token_str = line.substr(column_number_start, column_number_end - column_number_start);
			TokenType token_type = TOKEN_WORD;
			if (is_number(token_str))
				token_type = TOKEN_INT;

			tokens.push_back(Token(
				token_str, token_type,
				Location(line_number, column_number_start, column_number_end, line, filename)
			));
		}

		// get start position of next token
		column_number_start = find_next_token_start_col(column_number_end);
	}
}

void Lexer::tokenize() {
	File file(filename, FILE_READ);
	if (!file.exists()) {
		print_error("couldn't open file '" + filename + "': " + strerror(errno));
		exit(1);
	}

	line_number = 0;
	while (file == true) {
		line = file.readline();
		line_number++;
		tokenize_line();
	}
}
