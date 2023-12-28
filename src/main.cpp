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

	TypeChecker type_checker(parser.program);
	type_checker.verify();
	type_checker.perform_checks();

	compile_to_asm(type_checker.program, "/tmp/out.asm");
	exit_on_error(std::system("fasm /tmp/out.asm ./a.out"));	

	return 0;
}
