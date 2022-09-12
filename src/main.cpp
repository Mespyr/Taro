/* #define DEBUG */

#include <iostream>
#include <vector>
#include "include/checks.h"
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

	#ifdef DEBUG
		for (Token t : tokens)
			std::cout << "value: " << t.value << " | line number: " << t.loc.line_number << std::endl;
	#endif

	std::cout << "[note] parsing tokens" << std::endl;
	Program program = parse_tokens(tokens);

	#ifdef DEBUG
		for (std::pair<std::string, Function> func : program.functions)
		{
			std::cout << "'" << func.first << "' args: " << func.second.arg_stack.size() << " ret: " << func.second.ret_stack.size() << std::endl;
			std::cout << "\t ops: " << func.second.ops.size() << " addr: " << func.second.addr << std::endl;
		}
	#endif

	std::cout << "[note] type-checking program" << std::endl;
	verify_program(program);
	type_check_program(program);

	std::cout << "[note] compiling to assembly" << std::endl;
	compile_to_asm(program, "out.asm");

	std::cout << "[cmd] fasm out.asm" << std::endl;
	exit_on_error(std::system("fasm out.asm"));	

	return 0;
}
