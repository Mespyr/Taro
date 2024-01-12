#include "assembly.h"

std::string register_to_string(AsmRegister reg) {
	std::string output;

	switch (reg) {
	case REGISTER_RAX:
		return "rax";
	case REGISTER_RBX:
		return "rbx";
	case REGISTER_RCX:
		return "rcx";
	case REGISTER_RSP:
		return "rsp";
	case REGISTER_RDI:
		return "rdi";
	case REGISTER_RSI:
		return "rsi";
	case REGISTER_RDX:
		return "rdx";
	case REGISTER_R10:
		return "r10";
	case REGISTER_R8:
		return "r8";
	case REGISTER_R9:
		return "r9";

	case REGISTER_AL:	
		return "al";
	case REGISTER_BL:	
		return "bl";
	case REGISTER_CL:	
		return "cl";
	case REGISTER_DL:
		return "dl";
	}
}

void AssemblyProgram::start_new_function(std::string func_name) {
	code.insert({func_name, {}});
	current_func_name = func_name;
}

void AssemblyProgram::new_inst(Instruction inst) {
	code.at(current_func_name).push_back(inst);
}
