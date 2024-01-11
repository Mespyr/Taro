#pragma once

#include <string>
#include <assert.h>
#include <sstream>
#include <vector>
#include "../include/program.h"
#include "../include/op.h"
#include "../include/file.h"
#include "../assembly/assembly.h"

class Compiler {
public:
	Compiler(Program program) :
		program(program)
	{}

	void generate_asm(std::string output_filename);
	void write_asm_to_file(std::string output_filename); // TODO: impl
private:
	Program program;
	AssemblyProgram asm_program;
};
