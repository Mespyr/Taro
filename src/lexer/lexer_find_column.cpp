#include "lexer.h"

long unsigned int Lexer::find_next_token_start_col(long unsigned int column_number) {
	while (column_number < line.length() && std::isspace(line.at(column_number)) && line.at(column_number) != '#')
		column_number++;
	return column_number;
}

long unsigned int Lexer::find_token_end_col(long unsigned int column_number) {
	while (column_number < line.length() && !std::isspace(line.at(column_number))
			&& line.at(column_number) != '#'
			&& line.at(column_number) != '(' && line.at(column_number) != ')'
			&& line.at(column_number) != '[' && line.at(column_number) != ']')
		column_number++;
	return column_number;
}

long unsigned int Lexer::find_string_end_col(long unsigned int column_number) {
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
