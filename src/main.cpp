#include <iostream>
#include "include/type_checker.h"
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

	Lexer lexer;
	lexer.set_file(argv[1]);
	lexer.tokenize();

	Parser parser(&lexer);
	parser.parse();

	std::cout << "[note] type-checking program" << std::endl;
	TypeChecker type_checker(parser.program);
	type_checker.verify();
	type_checker.perform_checks();

	std::cout << "[note] compiling to assembly" << std::endl;
	compile_to_asm(type_checker.program, "out.asm");

	std::cout << "[cmd] fasm out.asm" << std::endl;
	exit_on_error(std::system("fasm out.asm"));	

	return 0;
}
