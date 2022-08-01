#include "include/compiler.h"

void compile_to_asm(Program program, std::string output_filename)
{
	static_assert(OP_COUNT == 9, "unhandled op types in compile_to_asm()");

	File outfile(output_filename, FILE_WRITE);

	// boilerplate
	outfile.writeln("format ELF64 executable 3");
	outfile.writeln("segment readable executable");
	outfile.writeln("entry start");

	outfile.writeln("dump:");
	outfile.writeln("\tmov r9, -3689348814741910323");
	outfile.writeln("\tsub rsp, 40");
	outfile.writeln("\tmov BYTE [rsp+31], 10");
	outfile.writeln("\tlea rcx, [rsp+30]");
	outfile.writeln(".L2:");
	outfile.writeln("\tmov rax, rdi");
	outfile.writeln("\tlea r8, [rsp+32]");
	outfile.writeln("\tmul r9");
	outfile.writeln("\tmov rax, rdi");
	outfile.writeln("\tsub r8, rcx");
	outfile.writeln("\tshr rdx, 3");
	outfile.writeln("\tlea rsi, [rdx+rdx*4]");
	outfile.writeln("\tadd rsi, rsi");
	outfile.writeln("\tsub rax, rsi");
	outfile.writeln("\tadd eax, 48");
	outfile.writeln("\tmov BYTE [rcx], al");
	outfile.writeln("\tmov rax, rdi");
	outfile.writeln("\tmov rdi, rdx");
	outfile.writeln("\tmov rdx, rcx");
	outfile.writeln("\tsub rcx, 1");
	outfile.writeln("\tcmp rax, 9");
	outfile.writeln("\tja .L2");
	outfile.writeln("\tlea rax, [rsp+32]");
	outfile.writeln("\tmov edi, 1");
	outfile.writeln("\tsub rdx, rax");
	outfile.writeln("\txor eax, eax");
	outfile.writeln("\tlea rsi, [rsp+32+rdx]");
	outfile.writeln("\tmov rdx, r8");
	outfile.writeln("\tmov rax, 1");
	outfile.writeln("\tsyscall");
	outfile.writeln("\tadd rsp, 40");
	outfile.writeln("\tret");

	// compile functions
    for (auto fn_key = program.functions.begin(); fn_key != program.functions.end(); fn_key++)
    {
		// setup function
        Function function = fn_key->second;
		outfile.writeln("; function " + fn_key->first);
		outfile.writeln("func_addr_" + std::to_string(function.addr) + ":");
		outfile.writeln("\tmov [ret_stack_rsp], rsp");
		outfile.writeln("\tmov rsp, rax");

		// compile ops
		for (Op op : function.ops)
		{
			// debugging
			if (op.type == OP_DUMP)
			{
				outfile.writeln("\t; OP_DUMP");
				outfile.writeln("\tpop rdi");
				outfile.writeln("\tcall dump");
			}
			
			// arithmetics
			else if (op.type == OP_PLUS)
			{
				outfile.writeln("\t; OP_PLUS");
				outfile.writeln("\tpop rax");
				outfile.writeln("\tpop rbx");
				outfile.writeln("\tadd rbx, rax");
				outfile.writeln("\tpush rbx");
			} 
			else if (op.type == OP_MINUS)
			{
				outfile.writeln("\t; OP_MINUS");
				outfile.writeln("\tpop rax");
				outfile.writeln("\tpop rbx");
				outfile.writeln("\tsub rbx, rax");
				outfile.writeln("\tpush rbx");
			}
			else if (op.type == OP_MUL)
			{
				outfile.writeln("\t; OP_MUL");
				outfile.writeln("\tpop rax");
				outfile.writeln("\tpop rbx");
				outfile.writeln("\tmul rbx");
				outfile.writeln("\tpush rax");
			}
			else if (op.type == OP_DIV)
			{
				outfile.writeln("\t; OP_DIV");
				outfile.writeln("\txor rdx, rdx");
				outfile.writeln("\tpop rbx");
				outfile.writeln("\tpop rax");
				outfile.writeln("\tdiv rbx");
				outfile.writeln("\tpush rax");
				outfile.writeln("\tpush rdx");
			}

			// other
			else if (op.type == OP_PUSH_INT)
			{
				outfile.writeln("\tmov rax, " + std::to_string(op.int_operand));
				outfile.writeln("\tpush rax");
			}
			else if (op.type == OP_FUNCTION_CALL)
            {
                outfile.writeln("\t; OP_FUNCTION_CALL");
                outfile.writeln("\tmov rax, rsp");
                outfile.writeln("\tmov rsp, [ret_stack_rsp]");
                outfile.writeln("\tcall func_addr_" + std::to_string(program.functions.at(op.str_operand).addr));
                outfile.writeln("\tmov [ret_stack_rsp], rsp");
                outfile.writeln("\tmov rsp, rax");
            }
		}

		// end function
		outfile.writeln("\t; return");
		outfile.writeln("\tmov rax, rsp");
		outfile.writeln("\tmov rsp, [ret_stack_rsp]");
		outfile.writeln("\tret");
	}

	// entry code
	outfile.writeln("start:");
	outfile.writeln("\tmov rax, ret_stack_end");
	outfile.writeln("\tmov [ret_stack_rsp], rax");
	outfile.writeln("\tcall func_addr_" + std::to_string(program.functions.at("main").addr));
	outfile.writeln("\tmov rax, 60");
	outfile.writeln("\tmov rdi, 0");
	outfile.writeln("\tsyscall");

	// data
	outfile.writeln("segment readable writable");
    outfile.writeln("ret_stack_rsp: rq 1");
    outfile.writeln("ret_stack: rb 4096");
    outfile.writeln("ret_stack_end:");
}
