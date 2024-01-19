#pragma once

#include <cstdint>
#include <string>
#include <assert.h>
#include <sstream>
#include <vector>
#include <complex>
#include <map>
#include "../include/program.h"
#include "../include/op.h"
#include "../include/file.h"
#include "../assembly/assembly.h"

class Compiler {
public:
	Compiler(Program program) :
		program(program)
	{}

	void generate_asm();
	void perform_optimizations();
	void write_asm_to_file(std::string output_filename); // TODO: impl
private:
	void gen_op_asm(Op op, int func_addr);

	void handle_write_data_to_ptr(Op op);
	void handle_copy_ptr_to_ptr(Op op);
	void handle_read_data_from_ptr(Op op);

	void optimize_current_func();
	void remove_stored_register(AsmRegister reg);
	void handle_mov_reg_optim();

	Program program;
	AssemblyProgram asmp;

	std::map<AsmRegister, int64_t> stored_registers;
	uint32_t idx = 0;
	std::pair<std::string, std::vector<Instruction>> fn_key;
};
