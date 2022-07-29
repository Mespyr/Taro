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
	Function(Location loc) :
		loc(loc)
	{}

	Location loc;
	std::vector<Op> ops;
};

class Program
{
public:
	std::map<std::string, Function> functions;
};

#endif
