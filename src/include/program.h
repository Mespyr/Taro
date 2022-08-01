#ifndef _LCP_PROGRAM_H_
#define _LCP_PROGRAM_H_

#include <vector> 
#include <string>
#include <map>
#include "op.h"
#include "location.h"

// stores ops for a function
class Function
{
public:
	Function(Location loc, int addr) :
		loc(loc), addr(addr)
	{}

	Location loc;
	std::vector<Op> ops;
	int addr;
};

class Program
{
public:
	std::map<std::string, Function> functions;
};

#endif
