#include "include/error.h"

void print_error(std::string message)
{
	std::cerr << "ERROR: " << message << std::endl;
}

void print_error_at_loc(Location loc, std::string message)
{
	std::cerr << loc.file_location << ":" << loc.line_number << ":" << loc.column_number_start << ": " << message << std::endl;
	std::cerr << loc.line << std::endl;

	for (long unsigned int i = 0; i < loc.column_number_start; i++)
	{
		if (loc.line.at(i) == '\t')
			std::cerr << "\t";
		else std::cerr << " ";
	}

	for (long unsigned int i = loc.column_number_start; i < loc.column_number_end; i++)
	{
		if (loc.line.at(i) == '\t')
			std::cerr << "\t";
		else std::cerr << "^";
	}

	std::cerr << std::endl;
}
