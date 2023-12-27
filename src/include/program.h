#pragma once

#include <vector> 
#include <string>
#include <map>
#include "op.h"
#include "location.h"

// PROGRAM TYPES

enum RambutanPrimType {
	TYPE_I8,
	TYPE_I64,
	PRIM_TYPES_COUNT
};

enum StackSizeType {
	STACK_SIZE_SINGLE,
	STACK_SIZE_TUPLE,
	STACK_SIZE_COUNT
};

class RambutanType {
public:
	RambutanType(Location loc) :
		loc(loc)
	{}
	RambutanType(Location loc, std::string type_str);
	RambutanType(Location loc, std::string base_type, int ptr_to_trace) :
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

#include "error.h"

// PROGRAM BLOCKS

class Const {
public:
	Const(Location loc, long long value) :
		loc(loc),
		value(value)
	{}

	Location loc;
	long long value;
};

class Function {
public:
	Function(Location loc, int addr, std::vector<RambutanType> arg_stack, std::vector<RambutanType> ret_stack) :
		loc(loc), addr(addr), arg_stack(arg_stack), ret_stack(ret_stack)
	{}

	Location loc;
	int addr;
	std::vector<RambutanType> arg_stack;
	std::vector<RambutanType> ret_stack;
	std::vector<Op> ops;
	std::map<std::string, std::pair<RambutanType, int>> var_offsets;
	int memory_capacity;
};

class Struct {
public:
	Struct(Location loc, std::map<std::string, std::pair<RambutanType, int>> members, int size) :
		loc(loc), members(members), size(size)
	{}

	Location loc;
	std::map<std::string, std::pair<RambutanType, int>> members;
	int size;
};

class Program {
public:
	std::map<std::string, Function> functions;
	std::map<std::string, Const> consts;
	std::map<std::string, Struct> structs;
};

// UTIL

std::vector<std::string> split_by_dot(std::string str);
std::pair<std::string, int> parse_type_str(std::string str);
std::string human_readable_type(RambutanType t);

std::pair<RambutanType, int> struct_member_offset(Op op, std::map<std::string, Struct> structs);
std::pair<RambutanType, int> variable_member_offset(Op op, std::map<std::string, std::pair<RambutanType, int>> var_offsets, std::map<std::string, Struct> structs);

std::string prim_type_name(RambutanPrimType type);

int sizeof_type(RambutanType type, std::map<std::string, Struct> structs = {});
int sizeof_type(std::string type, std::map<std::string, Struct> structs = {});

bool types_equal(RambutanType a, RambutanType b);

bool is_prim_type(RambutanType t);
bool is_prim_type(std::string t);

bool is_prim_type_int(RambutanType t);
bool is_prim_type_int(std::string t);

bool is_pointer(RambutanType t);
bool is_pointer(std::string t);
