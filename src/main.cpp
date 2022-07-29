#include <iostream>
#include <vector>
#include "include/token.h"
#include "include/file.h"
#include "include/lexer.h"
#include "include/program.h"
#include "include/parser.h"

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		std::cerr << "Error: No file provided for compilation" << std::endl;
		return 1;
	}

	std::vector<Token> tokens = tokenize_file(argv[1]);
	Program program = parse_tokens(tokens);

	for (auto fn_key = program.functions.begin(); fn_key != program.functions.end(); fn_key++)
	{
		Function function = fn_key->second;
		std::cout << "#####################" << std::endl;

		for (Op op : function.ops)
		{
			std::cout << op.type << " " << op.loc.line_number << op.loc.line << std::endl;
		}

	}

	return 0;
}
