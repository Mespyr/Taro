#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "location.h"
#include "op.h"

enum LCPPrimType {
	TYPE_I8,
	TYPE_I64,
	PRIM_TYPES_COUNT
};

enum StackSizeType {
	STACK_SIZE_SINGLE,
	STACK_SIZE_TUPLE,
	STACK_SIZE_COUNT
};

class LCPType {
public:
	LCPType(Location loc) :
		loc(loc)
	{}
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
	StackSizeType stack_type = STACK_SIZE_SINGLE;
};

#include "error.h" // error.h needs that LCPType class

// struct types
class Struct {
public:
	Struct(Location loc, std::map<std::string, std::pair<LCPType, int>> members, int size) :
		loc(loc), members(members), size(size)
	{}

	Location loc;
	std::map<std::string, std::pair<LCPType, int>> members;
	int size;
};

std::vector<std::string> split_by_dot(std::string str);
std::pair<std::string, int> parse_type_str(std::string str);
std::string human_readable_type(LCPType t);

std::pair<LCPType, int> struct_member_offset(Op op, std::map<std::string, Struct> structs);
std::pair<LCPType, int> variable_member_offset(Op op, std::map<std::string, std::pair<LCPType, int>> var_offsets, std::map<std::string, Struct> structs);

std::string prim_type_name(LCPPrimType type);

int sizeof_type(LCPType type, std::map<std::string, Struct> structs = {});
int sizeof_type(std::string type, std::map<std::string, Struct> structs = {});

bool types_equal(LCPType a, LCPType b);

bool is_prim_type(LCPType t);
bool is_prim_type(std::string t);

bool is_prim_type_int(LCPType t);
bool is_prim_type_int(std::string t);

bool is_pointer(LCPType t);
bool is_pointer(std::string t);
