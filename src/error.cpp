#include "include/error.h"

void print_error(std::string message) {
	std::cerr << "ERROR: " << message << std::endl;
}

void print_error_at_loc(Location loc, std::string message) {
	std::cerr << "┌[" << loc.file_location << ":" << loc.line_number << ":" << loc.column_number_start << "]\n";
	std::cerr << "│ " << loc.line << "\n└ ";

	for (long unsigned int i = 0; i < loc.column_number_start; i++) {
		if (loc.line.at(i) == '\t')
			std::cerr << "\t";
		else std::cerr << " ";
	}

	for (long unsigned int i = loc.column_number_start; i < loc.column_number_end; i++) {
		if (loc.line.at(i) == '\t')
			std::cerr << "\t";
		else std::cerr << "^";
	}

	std::cerr << " " << message << std::endl;
}

void print_note_at_loc(Location loc, std::string message) {
	std::cerr << "┌[" << loc.file_location << ":" << loc.line_number << ":" << loc.column_number_start << "]\n";
	std::cerr << "│ " << loc.line << "\n└ ";

	for (long unsigned int i = 0; i < loc.column_number_start; i++) {
		if (loc.line.at(i) == '\t')
			std::cerr << "\t";
		else std::cerr << " ";
	}

	for (long unsigned int i = loc.column_number_start; i < loc.column_number_end; i++) {
		if (loc.line.at(i) == '\t')
			std::cerr << "\t";
		else std::cerr << "^";
	}

	std::cerr << " NOTE: " << message << std::endl;
}

void print_not_enough_arguments_error(Location loc, int expected, int actual, std::string name, std::string fullname, bool is_func) {
		if (is_func) 
			// not enough items on stack for function 'name' (expected 'expected', got 'actual')
			print_error_at_loc(loc, "not enough items on stack for function '" + name + "' (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
		else if (fullname == "")
			// not enough items on stack for operation 'name' (expected 'expected', got 'actual')
			print_error_at_loc(loc, "not enough items on stack for operation '" + name + "' (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
		else
			// not enough items on stack for operation 'name' (fullname) (expected 'expected', got 'actual')
			print_error_at_loc(loc, "not enough items on stack for operation '" + name + "' (" + fullname + ") (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
}

void print_invalid_combination_of_types_error(Location loc, std::vector<RambutanType> types, std::string name, std::string fullname, bool is_func) {
	// ex: 'int, int, ptr, int'
	std::string types_str;
	for (unsigned long int i = 0; i < types.size() - 1; i++)
		types_str.append(human_readable_type(types.at(i)) + ", ");
	types_str.append(human_readable_type(types.back()));
	
	if (is_func)
		// invalid types for function 'name' (fullname) (types_str)
		print_error_at_loc(loc, "invalid types for function '" + name + "' (" + types_str + ")");
	else if (fullname == "")
		// invalid types for operation 'name' (types_str)
		print_error_at_loc(loc, "invalid types for operation '" + name + "' (" + types_str + ")");
	else
		// invalid types for operation 'name' (fullname) (types_str)
		print_error_at_loc(loc, "invalid types for operation '" + name + "' (" + fullname + ") (" + types_str + ")");
}

void print_invalid_type_error(Location loc, std::string expected, std::string actual, std::string name, std::string fullname, bool is_func) {
	if (is_func)
		// invalid argument type for function 'name' (expected 'expected', got 'actual')
		print_error_at_loc(loc, "invalid type for function '" + name + "' (expected " + expected + ", got " + actual + ")");
	else if (fullname == "")
		// invalid argument type for operation 'name' (expected 'expected', got 'actual')
		print_error_at_loc(loc, "invalid type for operation '" + name + "' (expected " + expected + ", got " + actual + ")");
	else
		// invalid argument type for operation 'name' (fullname) (expected 'expected', got 'actual')
		print_error_at_loc(loc, "invalid type for operation '" + name + "' (" + fullname + ") (expected " + expected + ", got " + actual + ")");
}
