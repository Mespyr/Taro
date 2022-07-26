#include <iostream>
#include <vector>
#include "include/token.h"
#include "include/file.h"
#include "include/lexer.h"

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		std::cerr << "Error: No file provided for compilation" << std::endl;
		return 1;
	}

	std::vector<Token> tokens = tokenize_file(argv[1]);
	
	for (Token t : tokens) std::cout << t.value << " " << t.loc.line_number << std::endl;

	return 0;
}
