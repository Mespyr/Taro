#include "include/types.h"

std::string human_readable_type(DATATYPE type)
{
	static_assert(DATATYPE_COUNT == 2, "unhandled datatypes in human_readable_type()");
	
	if (type == DATATYPE_INT) return "int";
	if (type == DATATYPE_PTR) return "ptr";

	std::cerr << "Compiler Error: human_readable_type() is broken for some reason and is recieving bad input from the parser" << std::endl;
	exit(1);
}
