#include "assembly.h"

// backend
Instruction AssemblyProgram::inst_1_arity_backend(InstructionType type, Argument arg) {
	Instruction i(type);
	i.arguments.push_back(arg);
	return i;
}

Instruction AssemblyProgram::inst_2_arity_backend(InstructionType type, Argument arg1, Argument arg2) {
	Instruction i(type);
	i.arguments.push_back(arg1);
	i.arguments.push_back(arg2);
	return i;
}

Instruction AssemblyProgram::inst_name_arg_backend(InstructionType type, std::string name) {
	return Instruction(type, name);
}

// frontent
Instruction AssemblyProgram::inst_push(Argument arg) {
	return inst_1_arity_backend(INSTRUCTION_PUSH, arg);
}

Instruction AssemblyProgram::inst_pop(Argument arg) {
	return inst_1_arity_backend(INSTRUCTION_POP, arg);
}

Instruction AssemblyProgram::inst_mov(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_MOV, arg1, arg2);
}

Instruction AssemblyProgram::inst_add(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_ADD, arg1, arg2);
}

Instruction AssemblyProgram::inst_sub(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_SUB, arg1, arg2);
}

Instruction AssemblyProgram::inst_imul(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_IMUL, arg1, arg2);
}

Instruction AssemblyProgram::inst_div(Argument arg) {
	return inst_1_arity_backend(INSTRUCTION_DIV, arg);
}

Instruction AssemblyProgram::inst_cmp(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_CMP, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmove(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_CMOVE, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmovg(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_CMOVG, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmovl(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_CMOVL, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmovge(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_CMOVGE, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmovle(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_CMOVLE, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmovne(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_CMOVNE, arg1, arg2);
}

Instruction AssemblyProgram::inst_and(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_AND, arg1, arg2);
}

Instruction AssemblyProgram::inst_or(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_OR, arg1, arg2);
}

Instruction AssemblyProgram::inst_xor(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_XOR, arg1, arg2);
}

Instruction AssemblyProgram::inst_test(Argument arg1, Argument arg2) {
	return inst_2_arity_backend(INSTRUCTION_TEST, arg1, arg2);
}

Instruction AssemblyProgram::inst_label(std::string label_name) {
	return inst_name_arg_backend(INSTRUCTION_LABEL, label_name);
}

Instruction AssemblyProgram::inst_jz(std::string label_name) {
	return inst_name_arg_backend(INSTRUCTION_JZ, label_name);
}

Instruction AssemblyProgram::inst_jnz(std::string label_name) {
	return inst_name_arg_backend(INSTRUCTION_JNZ, label_name);
}

Instruction AssemblyProgram::inst_jmp(std::string label_name) {
	return inst_name_arg_backend(INSTRUCTION_JMP, label_name);
}

Instruction AssemblyProgram::inst_call(std::string func_name) {
	return inst_name_arg_backend(INSTRUCTION_CALL, func_name);
}

Instruction AssemblyProgram::inst_syscall() {
	return Instruction(INSTRUCTION_SYSCALL);
}
