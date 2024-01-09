#include "../include/assembly.h"

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
	case REGISTER_RBP:
		return "rbp";
	case REGISTER_RDI:
		return "rdi";
	case REGISTER_RSI:
		return "rsi";
	case REGISTER_RDX:
		return "rdx";

	case REGISTER_EAX:
		return "eax";
	case REGISTER_EBX:
		return "ebx";
	case REGISTER_ECX:
		return "ecx";
	case REGISTER_EDI:
		return "edi";
	case REGISTER_ESI:
		return "esi";
	case REGISTER_EDX:
		return "edx";

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
