#include "include/program.h"

LangType::LangType(Location loc, std::string type_str) : loc(loc) {
	std::pair<std::string, int> pair = parse_type_str(type_str);
	ptr_to_trace = pair.second;
	base_type = pair.first;
}

std::vector<std::string> split_by_dot(std::string str) {
	str += ".";
	std::vector<std::string> split;
	uint64_t pos = str.find(".");
	while (pos != std::string::npos) {
		split.push_back(str.substr(0, pos)); // push back everything before the period
		str = str.substr(pos+1); // set the string to everything after the period
		pos = str.find("."); // find the next period
	}
	return split;
}

std::pair<std::string, int> parse_type_str(std::string str) {
	int i = 0;

	// strip base_type of all '^' and inc ptr_to_trace for each
	while (str.at(0) == '^') {
		i++;
		str = str.substr(1);
	}

	return {str, i};
}

std::string human_readable_type(LangType t) {
	std::string str;
	for (int i = 0; i < t.ptr_to_trace; i++)
		str.push_back('^');

	return str + t.base_type;
}

std::pair<LangType, int> struct_member_offset(Op op, std::map<std::string, Struct> structs) {
	static_assert(PRIM_TYPES_COUNT == 2, "unhandled prim types in get_struct_member()");

	std::vector<std::string> split_member_path = split_by_dot(op.str_operand);
	std::string type_name = split_member_path.front();
	
	if (structs.count(type_name)) {
		std::map<std::string, std::pair<LangType, int>> struct_members = structs.at(type_name).members;
		int offset = 0; // set offset to start of variable
		uint64_t current_member_idx = 1;
		LangType member_type(op.loc);

		while (current_member_idx < split_member_path.size()) {
			if (!struct_members.count(split_member_path.at(current_member_idx))) {
				print_error_at_loc(op.loc, "struct '" + type_name + "' doesn't have the member '" + split_member_path.at(1) + "' defined");	
				exit(1);
			}
			std::pair<LangType, int> member_type_offset_pair = struct_members.at(split_member_path.at(current_member_idx));
			// increment offset by the relative offset of the member
			offset += member_type_offset_pair.second;
			// if the member is a struct and isn't the last member of the member path
			// access the members of the member's type and continue
			if (structs.count(member_type_offset_pair.first.base_type) && member_type_offset_pair.first.ptr_to_trace == 0 && current_member_idx < split_member_path.size() - 1) {
				struct_members = structs.at(member_type_offset_pair.first.base_type).members;
				current_member_idx++;
			}
			else {
				member_type = member_type_offset_pair.first;
				break;
			}
		}
		return {member_type, offset};
	}
	print_error_at_loc(op.loc, "compiler error: non-existant struct type was passed into the function struct_member_offset() when this function checks for the ");
	exit(1);
}

std::pair<LangType, int> variable_member_offset(Op op, std::map<std::string, std::pair<LangType, int>> var_offsets, std::map<std::string, Struct> structs) {
	static_assert(PRIM_TYPES_COUNT == 2, "unhandled prim types in get_struct_member()");

	std::vector<std::string> split_member_path = split_by_dot(op.str_operand);
	std::string var_name = split_member_path.front();
	if (var_offsets.count(var_name)) {
		if (split_member_path.size() == 1) {
			print_error_at_loc(op.loc, "member name wasn't provided in the 'set struct member' intrinsic");
			exit(1);
		}
		if (var_offsets.at(var_name).first.ptr_to_trace > 0) {
			print_error_at_loc(op.loc, "cannot access struct members from a variable saved as a pointer to the struct");
			exit(1);
		}
		std::map<std::string, std::pair<LangType, int>> struct_members = structs.at(var_offsets.at(var_name).first.base_type).members;
		int offset = var_offsets.at(var_name).second; // set offset to start of variable
		uint64_t current_member_idx = 1;
		LangType member_type(op.loc);

		while (current_member_idx < split_member_path.size()) {
			if (!struct_members.count(split_member_path.at(current_member_idx))) {
				print_error_at_loc(op.loc, "struct '" + var_offsets.at(var_name).first.base_type + "' doesn't have the member '" + split_member_path.at(1) + "' defined");	
				exit(1);
			}
			std::pair<LangType, int> member_type_offset_pair = struct_members.at(split_member_path.at(current_member_idx));
			// increment offset by the relative offset of the member
			offset += member_type_offset_pair.second;
			// if the member is a struct and isn't the last member of the member path
			// access the members of the member's type and continue
			if (structs.count(member_type_offset_pair.first.base_type) && member_type_offset_pair.first.ptr_to_trace == 0 && current_member_idx < split_member_path.size() - 1) {
				struct_members = structs.at(member_type_offset_pair.first.base_type).members;
				current_member_idx++;
			}
			else {
				member_type = member_type_offset_pair.first;
				break;
			}
		}
		return {member_type, offset};
	}
	print_error_at_loc(op.loc, "variable '" + var_name + "' does not exist");
	exit(1);
}

std::string prim_type_name(LangPrimType type) {
	switch(type) {
		case TYPE_INT:
			return "Int";
			break;
		case TYPE_CHAR:
			return "Char";
			break;
		case PRIM_TYPES_COUNT:
			print_error("DONT PASS PRIM_TYPES_COUNT INTO prim_type_size()");
			exit(1);
	}
	exit(1);
}

int sizeof_type(LangType type, std::map<std::string, Struct> structs) {
	if (is_pointer(type)) return 8;
	if (is_prim_type(type)) {
		static_assert(PRIM_TYPES_COUNT == 2, "unhandled prim types in is_prim_type(LCPType)");
		
		if (type.base_type == prim_type_name(TYPE_CHAR)) return 1;
		if (type.base_type == prim_type_name(TYPE_INT)) return 8;
	}
	return structs.at(type.base_type).size;
}
int sizeof_type(std::string type, std::map<std::string, Struct> structs) {
	std::pair<std::string, int> t = parse_type_str(type);
	// t.first is type name
	// t.second is pointer count
	if (t.second > 0) return 8;
	if (is_prim_type(t.first)) {
		static_assert(PRIM_TYPES_COUNT == 2, "unhandled prim types in is_prim_type(LCPType)");
		
		if (t.first == prim_type_name(TYPE_CHAR)) return 1;
		if (t.first == prim_type_name(TYPE_INT)) return 8;
	}
	return structs.at(t.first).size;
}

bool types_equal(LangType a, LangType b) {
	return (a.ptr_to_trace == b.ptr_to_trace && a.base_type == b.base_type);
}

bool is_prim_type(LangType t) {
	static_assert(PRIM_TYPES_COUNT == 2, "unhandled prim types in is_prim_type(LCPType)");

	if (t.base_type == prim_type_name(TYPE_INT) ||
		t.base_type == prim_type_name(TYPE_CHAR))
		return true;

	return false;
}
bool is_prim_type(std::string t) {
	static_assert(PRIM_TYPES_COUNT == 2, "unhandled prim types in is_prim_type(std::string)");

	std::string base_t = parse_type_str(t).first;
	if (base_t == prim_type_name(TYPE_INT) ||
		base_t == prim_type_name(TYPE_CHAR))
		return true;

	return false;
}

bool is_prim_type_int(LangType t) {
	return (t.ptr_to_trace == 0 && (t.base_type == prim_type_name(TYPE_INT) || t.base_type == prim_type_name(TYPE_CHAR)));
}

bool is_prim_type_int(std::string t) {
	return (t == prim_type_name(TYPE_INT) || t == prim_type_name(TYPE_CHAR));
}

bool is_pointer(LangType t) {
	return (t.ptr_to_trace > 0);
}

bool is_pointer(std::string t) {
	int pcount = parse_type_str(t).second;
	return (pcount > 0);
}
