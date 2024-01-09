#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <map>

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
	INSTRUCTION_XOR,
	
	INSTRUCTION_CALL,
	INSTRUCTION_SYSCALL,
};

// all used registers (as far as I know)
enum AsmRegister {
	REGISTER_RAX,
	REGISTER_RBX,
	REGISTER_RCX,
	REGISTER_RSP,
	REGISTER_RBP,
	REGISTER_RDI,
	REGISTER_RSI,
	REGISTER_RDX,

	REGISTER_EAX,
	REGISTER_EBX,
	REGISTER_ECX,
	REGISTER_EDI,
	REGISTER_ESI,
	REGISTER_EDX,

	REGISTER_AL,
	REGISTER_BL,
	REGISTER_CL,
	REGISTER_DL,
};
std::string register_to_string(AsmRegister reg);

enum AsmSizeInference {
	INFER_QWORD,
	INFER_BYTE,
	INFER_NONE
};

enum ArgumentType {
	ARG_VARIABLE,
	ARG_INTEGER,
	ARG_REGISTER
};

class Argument {
public:
	Argument(std::string value, bool read_pointer = false, AsmSizeInference size_inference = INFER_NONE, long long add_amount = 0) :
		read_pointer(read_pointer), size_inference(size_inference), type(ARG_VARIABLE), var_value(value), add_amount(add_amount) 
	{}

	Argument(long long value, bool read_pointer = false, AsmSizeInference size_inference = INFER_NONE, long long add_amount = 0) :
		read_pointer(read_pointer), size_inference(size_inference), type(ARG_INTEGER), int_value(value), add_amount(add_amount) 
	{}

	Argument(AsmRegister value, bool read_pointer = false, AsmSizeInference size_inference = INFER_NONE, long long add_amount = 0) :
		read_pointer(read_pointer), size_inference(size_inference), type(ARG_REGISTER), reg_value(value), add_amount(add_amount) 
	{}

	bool read_pointer = false;
	AsmSizeInference size_inference;

	ArgumentType type;
	std::string var_value;
	long long int_value;
	AsmRegister reg_value;

	long long add_amount;

	std::string to_string();
};

class Instruction {
public:
	Instruction(InstructionType t) :
		type(t)
	{}

	InstructionType type;
	std::vector<Argument> arguments;

	std::string to_string();
};

class AssemblyProgram { // TODO: impl
public:
	std::map<std::string, std::vector<Instruction>> code;
};
