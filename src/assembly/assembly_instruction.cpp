#include "../include/assembly.h"

std::string Argument::to_string() {
	std::string output;

	switch (size_inference) {
	case INFER_BYTE:
		output.append("BYTE ");
		break;
	case INFER_QWORD:
		output.append("QWORD ");
		break;
	case INFER_NONE:
		break;
	}

	if (read_pointer)
		output.append("[");

	switch (type) {
	case ARG_INTEGER:
		output.append(std::to_string(int_value));
		break;
	case ARG_VARIABLE:
		output.append(var_value);
		break;
	case ARG_REGISTER:
		output.append(register_to_string(reg_value));
		break;
	}

	if (add_amount != 0)
		output.append("+" + std::to_string(add_amount));

	if (read_pointer)
		output.append("]");

	return output;
}

std::string Instruction::to_string() {
	std::string output;

	switch (type) {
	case INSTRUCTION_PUSH:
		output.append("push");
		break;
	case INSTRUCTION_POP:
		output.append("pop");
		break;
	case INSTRUCTION_MOV:
		output.append("mov");
		break;
	case INSTRUCTION_ADD:
		output.append("add");
		break;
	case INSTRUCTION_SUB:
		output.append("sub");
		break;
	case INSTRUCTION_IMUL:
		output.append("imul");
		break;
	case INSTRUCTION_DIV:
		output.append("div");
		break;
	case INSTRUCTION_CMP:
		output.append("cmp");
		break;
	case INSTRUCTION_CMOVE:
		output.append("cmove");
		break;
	case INSTRUCTION_CMOVG:
		output.append("cmovg");
		break;
	case INSTRUCTION_CMOVL:
		output.append("cmovl");
		break;
	case INSTRUCTION_CMOVGE:
		output.append("cmovge");
		break;
	case INSTRUCTION_CMOVLE:
		output.append("cmovle");
		break;
	case INSTRUCTION_CMOVNE:
		output.append("cmovne");
		break;
	case INSTRUCTION_AND:
		output.append("and");
		break;
	case INSTRUCTION_OR:
		output.append("or");
		break;
	case INSTRUCTION_JZ:
		output.append("jz");
		break;
	case INSTRUCTION_JNZ:
		output.append("jnz");
		break;
	case INSTRUCTION_JMP:
		output.append("jmp");
		break;
	case INSTRUCTION_TEST:
		output.append("test");
		break;
	case INSTRUCTION_XOR:
		output.append("xor");
		break;
	case INSTRUCTION_CALL:
		output.append("call");
		break;
	case INSTRUCTION_SYSCALL:
		output.append("syscall");
		break;
	}

	if (arguments.size() == 0)
		return output;

	output.append(" ");
	for (unsigned long int i = 0; i < arguments.size() - 1; i++)
		output.append(arguments.at(i).to_string() + ", ");
	output.append(arguments.back().to_string());

	return output;
}
