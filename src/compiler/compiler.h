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
	bool arguments_equal(Argument a, Argument b);
	void erase_idx(uint32_t i);
	void remove_stored_register(AsmRegister reg);

	void handle_mov_reg_optim();
	void compare_stored_values_mov(Instruction i, AsmRegister write_reg, int64_t save_value);

	bool register_is_used(AsmRegister reg, uint32_t end_idx);
    bool register_is_used_before_being_set(AsmRegister reg);
    int64_t find_idx_for_top_push();
	void handle_stack_pop_optim();
	void handle_stack_push_optim();

	Program program;
	AssemblyProgram asmp;

	std::map<AsmRegister, int64_t> stored_registers;
	uint32_t idx = 0;
	std::pair<std::string, std::vector<Instruction>> fn_key;
};
