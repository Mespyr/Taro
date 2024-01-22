#include "compiler.h"

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
