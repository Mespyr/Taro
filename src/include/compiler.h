#pragma once

#include <string>
#include <assert.h>
#include <sstream>
#include "program.h"
#include "op.h"
#include "file.h"

enum InstructionType {
	INSTRUCTION_PUSH,
	INSTRUCTION_POP,
	INSTRUCTION_MOV,

	INSTRUCTION_ADD,
	INSTRUCTION_SUB,
	INSTRUCTION_IMUL,
	INSTRUCTION_DIV,

	INSTRUCTION_CMP,
	INSTRUCTION_CMOVE,
	INSTRUCTION_CMOVG,
	INSTRUCTION_CMOVL,
	INSTRUCTION_CMOVGE,
	INSTRUCTION_CMOVLE,
	INSTRUCTION_CMOVNE,
	INSTRUCTION_AND,
	INSTRUCTION_OR,

	INSTRUCTION_JZ,
	INSTRUCTION_JNZ,
	INSTRUCTION_JMP,
	INSTRUCTION_TEST,
	
	INSTRUCTION_CALL,
	INSTRUCTION_SYSCALL,

	INSTRUCTION_COUNT,
};

class Instruction {
	InstructionType type;
};

class Compiler {
public:
	Compiler(Program program) :
		program(program)
	{}

	void generate_asm(std::string output_filename);
private:
	Program program;
	std::vector<Instruction> assembly_code;
};
