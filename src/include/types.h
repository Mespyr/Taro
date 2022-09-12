#ifndef _LCP_TYPES_H_
#define _LCP_TYPES_H_

#include <string>
#include <iostream>
#include "location.h"

enum DATATYPE
{
	DATATYPE_INT,
	DATATYPE_PTR,

	// count
	DATATYPE_COUNT
};

class TypeAtLoc
{
public:
	TypeAtLoc(Location loc, DATATYPE type) :
		loc(loc),
		type(type) 
	{}

	Location loc;
	DATATYPE type;
};

std::string human_readable_type(DATATYPE type);

#endif
