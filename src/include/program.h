#pragma once

#include <cstdint>
#include <vector> 
#include <string>
#include <map>
#include "op.h"
#include "location.h"

// PROGRAM TYPES

enum LangPrimType {
	TYPE_CHAR,
	TYPE_INT,
	PRIM_TYPES_COUNT
};

class LangType {
public:
	LangType(Location loc) :
		loc(loc)
	{}
	LangType(Location loc, std::string type_str);
	LangType(Location loc, std::string base_type, int ptr_to_trace) :
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

#include "error.h"

// PROGRAM BLOCKS

class Const {
public:
	Const(Location loc, int64_t value) :
		loc(loc),
		value(value)
	{}

	Location loc;
	int64_t value;
};

struct FunctionSignature {
	std::vector<LangType> argument_stack;
	std::vector<LangType> return_stack;
};

class Function {
public:
	Function(Location loc, int addr, FunctionSignature signature) :
		loc(loc), addr(addr), signature(signature) 
	{}

	Location loc;
	int addr;
	FunctionSignature signature;
	std::vector<Op> ops;
	std::map<std::string, std::pair<LangType, int>> var_offsets;
	int memory_capacity = 0;
};

class Struct {
public:
	Struct(Location loc, std::map<std::string, std::pair<LangType, int>> members, int size) :
		loc(loc), members(members), size(size)
	{}

	Location loc;
	std::map<std::string, std::pair<LangType, int>> members;
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
std::string human_readable_type(LangType t);

std::pair<LangType, int> struct_member_offset(Op op, std::map<std::string, Struct> structs);
std::pair<LangType, int> variable_member_offset(Op op, std::map<std::string, std::pair<LangType, int>> var_offsets, std::map<std::string, Struct> structs);

std::string prim_type_name(LangPrimType type);

int sizeof_type(LangType type, std::map<std::string, Struct> structs = {});
int sizeof_type(std::string type, std::map<std::string, Struct> structs = {});

bool types_equal(LangType a, LangType b);

bool is_prim_type(LangType t);
bool is_prim_type(std::string t);

bool is_prim_type_int(LangType t);
bool is_prim_type_int(std::string t);

bool is_pointer(LangType t);
bool is_pointer(std::string t);
