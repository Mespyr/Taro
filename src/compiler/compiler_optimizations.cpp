#include "compiler.h"
#include <iostream>

// zero-related optimizations
void Compiler::simple_optimize() {
	for (std::pair<std::string, std::vector<Instruction>> fn_key : asmp.code) {
		unsigned int idx = 0;
		for (Instruction i : fn_key.second) {
			switch (i.type) {
			case INSTRUCTION_ADD:
			case INSTRUCTION_SUB: {
				if (i.arguments.at(1).type == ARG_INTEGER && i.arguments.at(1).int_value == 0)
					fn_key.second.erase(fn_key.second.begin() + idx);
			} break;

			case INSTRUCTION_MOV: {
				if (i.arguments.at(1).type == ARG_INTEGER && i.arguments.at(1).int_value == 0) {
					i.arguments.pop_back();
					i.arguments.push_back(i.arguments.at(0));
					i.type = INSTRUCTION_XOR;
					fn_key.second.at(idx) = i;
				}
			} break;

			default:
				break;
			}
			idx++;
		}
		asmp.code.at(fn_key.first) = fn_key.second;
	}
}

void Compiler::perform_optimizations() {
	simple_optimize();
}
