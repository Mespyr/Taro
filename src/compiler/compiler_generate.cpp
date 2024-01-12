#include "compiler.h"

void Compiler::generate_asm() {
	static_assert(OP_COUNT == 57, "unhandled op types in compile_to_asm()");

	for (auto fn_key = program.functions.begin(); fn_key != program.functions.end(); fn_key++) {
		Function function = fn_key->second;
		asmp.start_new_function("func_addr_" + std::to_string(function.addr));
		asmp.new_inst(asmp.inst_sub(Argument(REGISTER_RSP), Argument(function.memory_capacity)));
		asmp.new_inst(asmp.inst_mov(Argument("ret_stack_rsp", true), Argument(REGISTER_RSP)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RSP), Argument(REGISTER_RAX)));

		for (Op op : function.ops)
			gen_op_asm(op, function.addr);

		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(REGISTER_RSP)));
		asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RSP), Argument("ret_stack_rsp", true)));
		asmp.new_inst(asmp.inst_add(Argument(REGISTER_RSP), Argument(function.memory_capacity)));
		asmp.new_inst(asmp.inst_ret());
	}

	// entry code
	asmp.start_new_function("start");
	asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument("ret_stack_end")));
	asmp.new_inst(asmp.inst_mov(Argument("ret_stack_rsp", true), Argument(REGISTER_RAX)));
	asmp.new_inst(asmp.inst_call("func_addr_" + std::to_string(program.functions.at("main").addr)));
	asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(60)));
	asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDI), Argument(0)));
	asmp.new_inst(asmp.inst_syscall());
}

void Compiler::write_asm_to_file(std::string output_filename) {
	asmp.write_to_file(output_filename);
}
