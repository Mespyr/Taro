#include "include/lexer.h"

bool is_number(std::string n)
{
	if (n.at(0) == '-')
		n = n.substr(1);

	std::string::const_iterator it = n.begin();
	while (it != n.end() && std::isdigit(*it)) 
		++it;

	return !n.empty() && it == n.end();
}

long unsigned int find_next_token_start_col(long unsigned int column_number, std::string line)
{
	while (column_number < line.length() && std::isspace(line.at(column_number)) && line.at(column_number) != '#')
		column_number++;

	return column_number;
}

long unsigned int find_token_end_col(long unsigned int column_number, std::string line)
{
	while (column_number < line.length() && !std::isspace(line.at(column_number))
			&& line.at(column_number) != '#'
			&& line.at(column_number) != '(' && line.at(column_number) != ')'
			&& line.at(column_number) != '[' && line.at(column_number) != ']')
		column_number++;

	return column_number;
}

long unsigned int find_string_end_col(long unsigned int column_number, std::string line)
{
	// start column_number after first quote
	column_number++;

	while (column_number < line.length() && line.at(column_number) != '"')
	{
		// check if escape char was found and skip next char from parsing if not at EOL
		if (line.at(column_number) == '\\')
			column_number++;

		column_number++;
	}

	// return position where '"' was found
	return column_number;
}

std::vector<Token> tokenize_line(std::string line, std::string file_location, long unsigned int line_number)
{
	static_assert(TOKEN_TYPE_COUNT == 3, "unhandled token types in tokenize_line()");

	std::vector<Token> tokens;

	// get starting position of first token
	long unsigned int column_number_start = find_next_token_start_col(0, line);
	long unsigned int column_number_end;

	while (column_number_start < line.length())
	{
		// if there is a comment, then ignore rest of line
		if (line.at(column_number_start) == '#') 
			return tokens;

		// if token is a string
		if (line.at(column_number_start) == '"')
		{
			// get end position of string
			column_number_end = find_string_end_col(column_number_start, line);

			// check if col_end is at end of line
			if (column_number_end == line.length())
			{
				print_error_at_loc(
					Location(line_number, column_number_start, column_number_end, line, file_location),
					"unexpected EOL while tokenizing string"
				);
				exit(1);
			}
			// inc col_end so it points to after '"'
			column_number_end++;

			tokens.push_back(Token(
				line.substr(column_number_start, column_number_end - column_number_start), TOKEN_STRING,
				Location(line_number, column_number_start, column_number_end, line, file_location)
			));
		}
		// if token is any of these chars ()[]
		else if (line.at(column_number_start) == '(' || line.at(column_number_start) == ')' || line.at(column_number_start) == '[' || line.at(column_number_start) == ']')
		{
			tokens.push_back(Token(
				line.substr(column_number_start, 1), TOKEN_WORD,
				Location(line_number, column_number_start, column_number_start+1, line, file_location)
			));
			// one after the char
			column_number_end = column_number_start + 1;
		}
		else
		{
			column_number_end = find_token_end_col(column_number_start, line);

			std::string token_str = line.substr(column_number_start, column_number_end - column_number_start);
			TokenType token_type = TOKEN_WORD;
			if (is_number(token_str))
				token_type = TOKEN_INT;

			tokens.push_back(Token(
				token_str, token_type,
				Location(line_number, column_number_start, column_number_end, line, file_location)
			));
		}

		// get start position of next token
		column_number_start = find_next_token_start_col(column_number_end, line);
	}

	return tokens;
}

std::vector<Token> tokenize_file(std::string file_location)
{
	// open file
	File file(file_location, FILE_READ);

	// exit if file doesn't exist
	if (!file.exists())
	{
		print_error("couldn't open file '" + file_location + "': " + strerror(errno));
		exit(1);
	}

	std::vector<Token> tokens;
	long unsigned int line_number = 0;

	while (file == true)
	{
		// read next line from file
		std::string line = file.readline();
		line_number++;

		// get tokens on line
		std::vector<Token> line_toks = tokenize_line(line, file_location, line_number);

		// add all tokens from line to main token vector
		tokens.insert(tokens.end(), line_toks.begin(), line_toks.end());

		// clear line_toks
		line_toks.clear();
	}

	return tokens;
}
