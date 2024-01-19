#include "lexer.h"

uint64_t Lexer::find_next_token_start_col(uint64_t column_number) {
	while (column_number < line.length() && std::isspace(line.at(column_number)) && line.at(column_number) != '#')
		column_number++;
	return column_number;
}

uint64_t Lexer::find_token_end_col(uint64_t column_number) {
	while (column_number < line.length() && !std::isspace(line.at(column_number))
			&& line.at(column_number) != '#'
			&& line.at(column_number) != '(' && line.at(column_number) != ')'
			&& line.at(column_number) != '[' && line.at(column_number) != ']')
		column_number++;
	return column_number;
}

uint64_t Lexer::find_string_end_col(uint64_t column_number) {
	// start column_number after first quote
	column_number++;
	while (column_number < line.length() && line.at(column_number) != '"') {
		// check if escape char was found and skip next char from parsing if not at EOL
		if (line.at(column_number) == '\\')
			column_number++;
		column_number++;
	}

	// return position where '"' was found
	return column_number;
}
