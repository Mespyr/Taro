#include <iostream>
#include "include/checks.h"
#include "include/token.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/compiler.h"

#define exit_on_error(exit_code) if (exit_code != 0) exit(exit_code);

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Error: No file provided for compilation" << std::endl;
		return 1;
	}

	std::cout << "[note] tokenizing file" << std::endl;
	Lexer lexer;
	lexer.set_file(argv[1]);
	lexer.tokenize();

	std::cout << "[note] parsing tokens" << std::endl;
	Parser parser(&lexer);
	parser.parse();

	std::cout << "[note] type-checking program" << std::endl;
	verify_program(parser.program);
	type_check_program(parser.program);

	std::cout << "[note] compiling to assembly" << std::endl;
	compile_to_asm(parser.program, "out.asm");

	std::cout << "[cmd] fasm out.asm" << std::endl;
	exit_on_error(std::system("fasm out.asm"));	

	return 0;
}
