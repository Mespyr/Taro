#include "assembly.h"

void AssemblyProgram::write_beginning_boilerplate(File* outfile) {
	outfile->writeln("format ELF64 executable 3");
	outfile->writeln("segment readable executable");
	outfile->writeln("entry start");

	outfile->writeln("dump:");
	outfile->writeln("\tmov r9, -3689348814741910323");
	outfile->writeln("\tsub rsp, 40");
	outfile->writeln("\tmov BYTE [rsp+31], 10");
	outfile->writeln("\tlea rcx, [rsp+30]");
	outfile->writeln(".L2:");
	outfile->writeln("\tmov rax, rdi");
	outfile->writeln("\tlea r8, [rsp+32]");
	outfile->writeln("\tmul r9");
	outfile->writeln("\tmov rax, rdi");
	outfile->writeln("\tsub r8, rcx");
	outfile->writeln("\tshr rdx, 3");
	outfile->writeln("\tlea rsi, [rdx+rdx*4]");
	outfile->writeln("\tadd rsi, rsi");
	outfile->writeln("\tsub rax, rsi");
	outfile->writeln("\tadd eax, 48");
	outfile->writeln("\tmov BYTE [rcx], al");
	outfile->writeln("\tmov rax, rdi");
	outfile->writeln("\tmov rdi, rdx");
	outfile->writeln("\tmov rdx, rcx");
	outfile->writeln("\tsub rcx, 1");
	outfile->writeln("\tcmp rax, 9");
	outfile->writeln("\tja .L2");
	outfile->writeln("\tlea rax, [rsp+32]");
	outfile->writeln("\tmov edi, 1");
	outfile->writeln("\tsub rdx, rax");
	outfile->writeln("\txor eax, eax");
	outfile->writeln("\tlea rsi, [rsp+32+rdx]");
	outfile->writeln("\tmov rdx, r8");
	outfile->writeln("\tmov rax, 1");
	outfile->writeln("\tsyscall");
	outfile->writeln("\tadd rsp, 40");
	outfile->writeln("\tret");
}

void AssemblyProgram::write_to_file(std::string filename) {
	File outfile(filename, FILE_WRITE);

	// start code segment
	write_beginning_boilerplate(&outfile);
	for (auto fn_key = code.begin(); fn_key != code.end(); fn_key++) {
		outfile.writeln(fn_key->first + ":");
		for (Instruction i : fn_key->second)
			outfile.writeln("\t" + i.to_string());
	}

	// start data segment
	outfile.writeln("segment readable writable");
	for (long unsigned int i = 0; i < string_data.size(); i++) {
		std::string str = string_data.at(i);
		std::stringstream ss;
		if (str.size() == 0)
			ss << "0x0";
		else {
			for (char a : str)
				ss << (int) a << ",";
			ss << 0;
		}
		outfile.writeln("str_" + std::to_string(i) + ": db " + ss.str());
	}

	outfile.writeln("ret_stack_rsp: rq 1");
	outfile.writeln("ret_stack: rb 4096");
	outfile.writeln("ret_stack_end:");
}
