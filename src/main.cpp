#include <iostream>
#include <vector>
#include "include/token.h"
#include "include/file.h"
#include "include/lexer.h"
#include "include/program.h"
#include "include/parser.h"
#include "include/compiler.h"

#define exit_on_error(exit_code) if (exit_code != 0) exit(exit_code);

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		std::cerr << "Error: No file provided for compilation" << std::endl;
		return 1;
	}

	std::cout << "[note] tokenizing file" << std::endl;
	std::vector<Token> tokens = tokenize_file(argv[1]);

	std::cout << "[note] parsing tokens" << std::endl;
	Program program = parse_tokens(tokens);

	std::cout << "[note] compiling to assembly" << std::endl;
	compile_to_asm(program, "out.asm");

	std::cout << "[cmd] fasm out.asm" << std::endl;
	exit_on_error(std::system("fasm out.asm"));	

	return 0;
}
