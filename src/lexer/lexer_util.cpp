#include "lexer.h"

void Lexer::set_file(std::string filename) {
	this->filename = filename;
	line_number = 0;
	tokens.clear();
}

bool Lexer::is_number(std::string n) {
	if (n.at(0) == '-')
		n = n.substr(1);

	std::string::const_iterator it = n.begin();
	while (it != n.end() && std::isdigit(*it)) 
		++it;

	return !n.empty() && it == n.end();
}
