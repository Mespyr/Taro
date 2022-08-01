#include <iostream>
#include <vector>
#include "include/token.h"
#include "include/file.h"
#include "include/lexer.h"
#include "include/program.h"
#include "include/parser.h"
#include "include/compiler.h"

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		std::cerr << "Error: No file provided for compilation" << std::endl;
		return 1;
	}

	std::vector<Token> tokens = tokenize_file(argv[1]);
	Program program = parse_tokens(tokens);
	compile_to_asm(program, "out.asm");

	return 0;
}
