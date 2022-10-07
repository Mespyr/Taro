#ifndef _LCP_TYPES_H_
#define _LCP_TYPES_H_

#include <string>
#include <vector>
#include <iostream>
#include "location.h"

enum LCP_Base_Type
{
	TYPE_I64,
	TYPE_I8
};

class LCPType
{
public:
	LCPType(Location loc, std::string type_str);
	LCPType(Location loc, std::string base_type, int ptr_to_trace) :
		loc(loc),
		base_type(base_type),
		ptr_to_trace(ptr_to_trace)
	{}

	Location loc;
	// base type of the thing (ex: int, or some custom type)
	std::string base_type;
	// the amount of pointers to the type (ex: ^^int is a pointer to a pointer to an int, so it would be 2)
	int ptr_to_trace = 0;
};

std::string get_base_type_name(LCP_Base_Type type);
std::pair<std::string, int> parse_type_str(std::string str);
std::string human_readable_type(LCPType t);
bool types_equal(LCPType a, LCPType b);
bool is_base_type_int(LCPType t);
bool is_base_type_int(std::string t);
bool is_pointer(LCPType t);

#endif
