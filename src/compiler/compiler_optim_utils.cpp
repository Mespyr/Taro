#include "compiler.h"

void Compiler::remove_stored_register(AsmRegister reg) {
	std::cout << "CLEAR " << register_to_string(reg) << ": " << fn_key.second.at(idx).to_string() << std::endl;
	for (auto it = stored_registers.begin(); it != stored_registers.end();)
		if (it->first == reg)
			it = stored_registers.erase(it);
		else
			++it;
}

bool Compiler::arguments_equal(Argument arg1, Argument arg2) {
	return arg1.to_string() == arg2.to_string();
}
