#pragma once

#include <cstdint>
#include <string>

class Location {
public:
	Location(uint32_t line_number, uint32_t column_number_start, uint32_t column_number_end, std::string line, std::string file_location) :
		line_number(line_number), 
		column_number_start(column_number_start), 
		column_number_end(column_number_end),
		line(line),
		file_location(file_location)
	{}

	uint32_t line_number, column_number_start, column_number_end;
	std::string line, file_location;
};
