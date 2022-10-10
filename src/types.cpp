#include "include/types.h"

LCPType::LCPType(Location loc, std::string type_str) : loc(loc)
{
	std::pair<std::string, int> pair = parse_type_str(type_str);
	ptr_to_trace = pair.second;
	base_type = pair.first;
}

std::vector<std::string> split_by_dot(std::string str)
{
	str += ".";
	std::vector<std::string> split;
	long unsigned int pos = str.find(".");
	while (pos != std::string::npos)
	{
		split.push_back(str.substr(0, pos)); // push back everything before the period
		str = str.substr(pos+1); // set the string to everything after the period
		pos = str.find("."); // find the next period
	}
	return split;
}

std::pair<LCPType, int> get_variable_type_offset(Op op, std::map<std::string, std::pair<LCPType, int>> var_offsets, std::map<std::string, Struct> structs)
{
	static_assert(BASE_TYPES_COUNT == 2, "unhandled base types in get_struct_member()");

	std::vector<std::string> split_member_path = split_by_dot(op.str_operand);
	// TODO: support basic types when they are able to be made into variables
	if (split_member_path.size() < 2)
	{
		print_error_at_loc(op.loc, "member name wasn't provided in the 'set struct member' intrinsic");
		exit(1);
	}
	std::string var_name = split_member_path.front();
	if (var_offsets.count(var_name))
	{
		std::map<std::string, std::pair<LCPType, int>> struct_members = structs.at(var_offsets.at(var_name).first.base_type).members;
		int offset = var_offsets.at(var_name).second; // set offset to start of variable
		unsigned long int current_member_idx = 1;
		LCPType member_type(op.loc);

		while (current_member_idx < split_member_path.size())
		{
			if (!struct_members.count(split_member_path.at(current_member_idx)))
			{
				print_error_at_loc(op.loc, "struct '" + var_offsets.at(var_name).first.base_type + "' doesn't have the member '" + split_member_path.at(1) + "' defined");	
				exit(1);
			}
			std::pair<LCPType, int> member_type_offset_pair = struct_members.at(split_member_path.at(current_member_idx));
			// increment offset by the relative offset of the member
			offset += member_type_offset_pair.second;
			// if the member is a struct and isn't the last member of the member path
			// access the members of the member's type and continue
			if (structs.count(member_type_offset_pair.first.base_type) && member_type_offset_pair.first.ptr_to_trace == 0 && current_member_idx < split_member_path.size() - 1)
			{
				// TODO: support other structs in accessing members by switching 'struct_members' variable to the members of the type
				struct_members = structs.at(member_type_offset_pair.first.base_type).members;
				current_member_idx++;
			}
			else
			{
				member_type = member_type_offset_pair.first;
				break;
			}
		}
		return {member_type, offset};
	}
	print_error_at_loc(op.loc, "variable '" + var_name + "' does not exist");
	exit(1);
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
			return "fuck you"; // except finn ofc
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
