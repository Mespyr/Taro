#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "../include/file.h"

enum InstructionType {
	INSTRUCTION_PUSH,
	INSTRUCTION_POP,
	INSTRUCTION_MOV,

	INSTRUCTION_ADD,
	INSTRUCTION_SUB,
	INSTRUCTION_IMUL,
	INSTRUCTION_DIV,
	INSTRUCTION_INC,
	INSTRUCTION_DEC,

	INSTRUCTION_CMP,
	INSTRUCTION_CMOVE,
	INSTRUCTION_CMOVG,
	INSTRUCTION_CMOVL,
	INSTRUCTION_CMOVGE,
	INSTRUCTION_CMOVLE,
	INSTRUCTION_CMOVNE,
	INSTRUCTION_AND,
	INSTRUCTION_OR,
	INSTRUCTION_TEST,
	INSTRUCTION_XOR,

	INSTRUCTION_LABEL, // not an instruction, but an indicator to place a label
	INSTRUCTION_JZ,
	INSTRUCTION_JNZ,
	INSTRUCTION_JMP,
	
	INSTRUCTION_CALL,
	INSTRUCTION_SYSCALL,
	INSTRUCTION_RET
};

// all used registers (as far as I know)
enum AsmRegister {
	REGISTER_RAX,
	REGISTER_RBX,
	REGISTER_RCX,
	REGISTER_RSP,
	REGISTER_RDI,
	REGISTER_RSI,
	REGISTER_RDX,
	REGISTER_R10,
	REGISTER_R8,
	REGISTER_R9,

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
	Argument(std::string value, bool read_pointer = false, AsmSizeInference size_inference = INFER_NONE, int64_t add_amount = 0) :
		read_pointer(read_pointer), size_inference(size_inference), type(ARG_VARIABLE), var_value(value), add_amount(add_amount) 
	{}

	Argument(int64_t value, bool read_pointer = false, AsmSizeInference size_inference = INFER_NONE, int64_t add_amount = 0) :
		read_pointer(read_pointer), size_inference(size_inference), type(ARG_INTEGER), int_value(value), add_amount(add_amount) 
	{}

	Argument(AsmRegister value, bool read_pointer = false, AsmSizeInference size_inference = INFER_NONE, int64_t add_amount = 0) :
		read_pointer(read_pointer), size_inference(size_inference), type(ARG_REGISTER), reg_value(value), add_amount(add_amount) 
	{}

	bool read_pointer = false;
	AsmSizeInference size_inference;

	ArgumentType type;
	std::string var_value;
	int64_t int_value;
	AsmRegister reg_value;

	int64_t add_amount;

	std::string to_string();
};

class Instruction {
public:
	Instruction(InstructionType t) :
		type(t)
	{}

	Instruction(InstructionType t, std::string arg_name) :
		type(t), arg_name(arg_name)
	{}

	InstructionType type;
	std::vector<Argument> arguments;
	std::string arg_name; // used by LABEL, JZ, JNZ, JMP, CALL
	
	std::string to_string();
};

class AssemblyProgram { // TODO: impl
public:
	bool compile_dump = false;
	std::map<std::string, std::vector<Instruction>> code;
	std::vector<std::string> string_data;

	void start_new_function(std::string func_name);
	void new_inst(Instruction inst);

	void write_to_file(std::string filename);
	
	Instruction inst_push(Argument arg);
	Instruction inst_pop(Argument arg);
	Instruction inst_mov(Argument arg1, Argument arg2);

	Instruction inst_add(Argument arg1, Argument arg2);
	Instruction inst_sub(Argument arg1, Argument arg2);
	Instruction inst_imul(Argument arg1, Argument arg2);
	Instruction inst_div(Argument arg);
	Instruction inst_inc(Argument arg);
	Instruction inst_dec(Argument arg);

	Instruction inst_cmp(Argument arg1, Argument arg2);
	Instruction inst_cmove(Argument arg1, Argument arg2);
	Instruction inst_cmovg(Argument arg1, Argument arg2);
	Instruction inst_cmovl(Argument arg1, Argument arg2);
	Instruction inst_cmovge(Argument arg1, Argument arg2);
	Instruction inst_cmovle(Argument arg1, Argument arg2);
	Instruction inst_cmovne(Argument arg1, Argument arg2);
	Instruction inst_and(Argument arg1, Argument arg2);
	Instruction inst_or(Argument arg1, Argument arg2);
	Instruction inst_xor(Argument arg1, Argument arg2);
	Instruction inst_test(Argument arg1, Argument arg2);

	Instruction inst_label(std::string label_name);
	Instruction inst_jz(std::string label_name);
	Instruction inst_jnz(std::string label_name);
	Instruction inst_jmp(std::string label_name);

	Instruction inst_call(std::string func_name);
	Instruction inst_syscall();
	Instruction inst_ret();
private:
	std::string current_func_name;
	void write_beginning_boilerplate(File* outfile);

	Instruction inst_1_arity_backend(InstructionType type, Argument arg);
	Instruction inst_2_arity_backend(InstructionType type, Argument arg1, Argument arg2);
	Instruction inst_name_arg_backend(InstructionType type, std::string name);
};
