#ifndef _LCP_PROGRAM_H_
#define _LCP_PROGRAM_H_

#include <vector> 
#include <string>
#include <map>
#include "op.h"
#include "location.h"
#include "types.h"

// stores ops for a function
class Function
{
public:
	Function(Location loc, int addr, std::vector<LCPType> arg_stack, std::vector<LCPType> ret_stack) :
		loc(loc), addr(addr), arg_stack(arg_stack), ret_stack(ret_stack)
	{}

	Location loc;
	int addr;
	std::vector<LCPType> arg_stack;
	std::vector<LCPType> ret_stack;
	std::vector<Op> ops;
	std::map<std::string, std::pair<LCPType, int>> var_offsets;
	int memory_capacity;
};

class Program
{
public:
	std::map<std::string, Function> functions;
	std::map<std::string, Struct> structs;
};

#endif
