#pragma once

#include <cstdint>
#include <string>

class Location {
  public:
    Location(uint32_t line_num, uint32_t start_column, uint32_t end_column,
             std::string line, std::string file_location)
        : line_num(line_num),
          start_column(start_column),
          end_column(end_column),
          line(line),
          file_location(file_location) {}

    uint32_t    line_num, start_column, end_column;
    std::string line, file_location;
};
