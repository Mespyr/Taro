#include <stdio.h>
#include "include/error.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/type_checker.h"
#include "include/compiler.h"
#include "include/assembly.h"

void exec(const char* cmd) {
    char buffer[128];
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
		print_error("popen() failed!");
		exit(1);
	}
    while (fgets(buffer, sizeof buffer, pipe) != nullptr);
    pclose(pipe);
}

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		print_error("Error: No file provided for compilation");
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

	Compiler compiler(type_checker.program);
	compiler.generate_asm("/tmp/out.asm");
	
	exec("fasm /tmp/out.asm ./a.out");

	// TEST
	Instruction i(INSTRUCTION_ADD);
	i.arguments.push_back(Argument(REGISTER_RAX));
	i.arguments.push_back(Argument("ret_stack_rsp", true, INFER_QWORD, 12));
	std::cout << i.to_string() << std::endl;
	
	return 0;
}
