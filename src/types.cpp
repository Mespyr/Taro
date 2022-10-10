#include "include/types.h"

LCPType::LCPType(Location loc, std::string type_str) :
	loc(loc)
{
	std::pair<std::string, int> pair = parse_type_str(type_str);
	ptr_to_trace = pair.second;
	base_type = pair.first;
}

std::string get_base_type_name(LCP_Base_Type type)
{
	switch(type)
	{
		case TYPE_I64:
			return "i64";
			break;
		case TYPE_I8:
			return "i8";
			break;
		case BASE_TYPES_COUNT:
			return "fuck you"; // really should not happen
			break;
	}
	exit(1);
}

std::pair<std::string, int> parse_type_str(std::string str)
{
	int i = 0;

	// strip base_type of all '^' and inc ptr_to_trace for each
	while (str.at(0) == '^')
	{
		i++;
		str = str.substr(1);
	}

	return {str, i};
}

std::string human_readable_type(LCPType t)
{
	std::string str;
	for (int i = 0; i < t.ptr_to_trace; i++)
		str.push_back('^');

	return str + t.base_type;
}

bool types_equal(LCPType a, LCPType b)
{
	return (a.ptr_to_trace == b.ptr_to_trace && a.base_type == b.base_type);
}

bool is_base_type_int(LCPType t)
{
	return (t.ptr_to_trace == 0 && (t.base_type == get_base_type_name(TYPE_I64) || t.base_type == get_base_type_name(TYPE_I8)));
}

bool is_base_type_int(std::string t)
{
	return (t == get_base_type_name(TYPE_I64) || t == get_base_type_name(TYPE_I8));
}

bool is_pointer(LCPType t)
{
	return (t.ptr_to_trace > 0);
}
