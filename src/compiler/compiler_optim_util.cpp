#include "compiler.h"
#include <cstdint>

bool Compiler::arguments_equal(Argument a, Argument b) {
	return a.to_string() == b.to_string();
}

void Compiler::erase_idx(uint32_t i) {
	fn_key.second.erase(fn_key.second.begin() + i);
	idx--;
}

void Compiler::remove_stored_register(AsmRegister reg) {
	//std::cout << "CLEAR " << register_to_string(reg) << ": " << fn_key.second.at(idx).to_string() << " | " << fn_key.first << std::endl;
	for (auto it = stored_registers.begin(); it != stored_registers.end();)
		if (it->first == reg)
			it = stored_registers.erase(it);
		else
			++it;
}

bool Compiler::register_is_used(AsmRegister reg, uint32_t end_idx) {
	for (uint32_t i = idx-1; i > end_idx; i--) {
		Instruction inst = fn_key.second.at(i);
		switch (inst.type) {

		// 2 arity
		case INSTRUCTION_ADD:
		case INSTRUCTION_SUB:
		case INSTRUCTION_IMUL:
		case INSTRUCTION_MOV:
		case INSTRUCTION_CMP:
		case INSTRUCTION_CMOVE:
		case INSTRUCTION_CMOVG:
		case INSTRUCTION_CMOVL:
		case INSTRUCTION_CMOVGE:
		case INSTRUCTION_CMOVLE:
		case INSTRUCTION_CMOVNE:
		case INSTRUCTION_AND:
		case INSTRUCTION_OR: {
			Argument a = inst.arguments.front();
			Argument b = inst.arguments.back();
			if ((a.type == ARG_REGISTER && a.reg_value == reg) ||
				(b.type == ARG_REGISTER && b.reg_value == reg))
				return true;
		} break;

		// 1 arity
		case INSTRUCTION_INC:
		case INSTRUCTION_DEC:
		case INSTRUCTION_PUSH:
		case INSTRUCTION_POP: {
			Argument a = inst.arguments.front();
			if (a.type == ARG_REGISTER && a.reg_value == reg)
				return true;
		} break;

		/* only one that really modifies more than js the register specified
		   without using another op before or after it
		 */
		case INSTRUCTION_DIV: {
			Argument a = inst.arguments.front();
			if ((a.type == ARG_REGISTER && a.reg_value == reg) ||
				(reg == REGISTER_RAX) || (reg == REGISTER_RDX))
				return true;
		} break;

		default:
			break;
		}
	}
	return false;
}

bool Compiler::register_is_used_before_being_set(AsmRegister reg) {
	// true -> register value was used
	// false -> register was set again without being used
	for (uint32_t i = idx+1; i < fn_key.second.size(); i++) {
		Instruction inst = fn_key.second.at(i);
		switch (inst.type) {

		// special
		case INSTRUCTION_POP: {
			Argument arg = inst.arguments.front();
			if (arg.type != ARG_REGISTER || arg.read_pointer)
				break;
			if (arg.reg_value == reg)
				return false;
		} break;
		case INSTRUCTION_PUSH: {
			Argument arg = inst.arguments.front();
			if (arg.type != ARG_REGISTER)
				break;
			if (arg.reg_value == reg)
				return true;
		} break;
		case INSTRUCTION_MOV: {
			Argument arg1 = inst.arguments.front();
			Argument arg2 = inst.arguments.front();
			if (arg1.type == ARG_REGISTER) {
				if (arg1.reg_value == reg) {
					// u are using the value inside the register to write data
					if (arg1.read_pointer) return true; 
					else return false;
				}
			}
			if (arg2.type == ARG_REGISTER && arg2.reg_value == reg)
				return true;
		} break;
		case INSTRUCTION_DIV: {
			// div uses rax, rbx and rdx
			// rbx is not changed, but rax and rdx are
			Argument arg = inst.arguments.front();
			if (arg.type == ARG_REGISTER && arg.reg_value == reg)
				return true; // rbx
			if (reg == REGISTER_RAX  || reg == REGISTER_RDX)
				return false;
		} break;

		// first-modifying 2-arity using first arg too
		case INSTRUCTION_ADD:
		case INSTRUCTION_SUB:
		case INSTRUCTION_IMUL:
		case INSTRUCTION_CMOVE:
		case INSTRUCTION_CMOVG:
		case INSTRUCTION_CMOVL:
		case INSTRUCTION_CMOVGE:
		case INSTRUCTION_CMOVLE:
		case INSTRUCTION_CMOVNE: {
			Argument arg1 = inst.arguments.front();
			Argument arg2 = inst.arguments.front();
			if ((arg1.type == ARG_REGISTER && arg1.reg_value == reg) ||
				(arg2.type == ARG_REGISTER && arg2.reg_value == reg))
				return true;
		} break;

		// non-modifying 2-arity
		case INSTRUCTION_CMP: {
			Argument arg1 = inst.arguments.front();
			Argument arg2 = inst.arguments.front();
			if (arg1.type == ARG_REGISTER) {
				if (arg1.reg_value == reg)
					return true;
			}
			if (arg2.type == ARG_REGISTER)
				if (arg2.reg_value == reg)
					return true;
		} break;

		case INSTRUCTION_CALL:
		case INSTRUCTION_LABEL:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JZ:
		case INSTRUCTION_JNZ:
		case INSTRUCTION_SYSCALL:
		case INSTRUCTION_RET: // JUST TO MAKE SURE IDK
			return false;

		default:
			break;
		}
	}
	return false;
}

int64_t Compiler::find_idx_for_top_push() {
	int32_t indent = 0;
	for (uint32_t i = idx-1; i > 0; i--) {
		Instruction inst = fn_key.second.at(i);
		switch (inst.type) {

		case INSTRUCTION_POP:
			indent++;
			break;
			
		case INSTRUCTION_PUSH: {
			if (indent > 0) indent--;
			else return i;
		} break;

		case INSTRUCTION_CALL:
		case INSTRUCTION_LABEL:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JZ:
		case INSTRUCTION_JNZ:
		case INSTRUCTION_SYSCALL:
		case INSTRUCTION_RET: // JUST TO MAKE SURE IDK
			return -1;

		default:
			break;
		}
	}

	return -1;
}
