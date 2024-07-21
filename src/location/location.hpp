#pragma once

#include <cstdint>
#include <memory>

#include "../file/file.hpp"

class Location {
  public:
    Location(std::shared_ptr<File> file, uint32_t line_num,
             uint32_t start_column, uint32_t end_column)
        : file(file),
          line_num(line_num),
          start_column(start_column),
          end_column(end_column) {}

    std::shared_ptr<File> file;

    uint32_t line_number();
    uint32_t start_col();
    uint32_t end_col();

  private:
    uint32_t line_num, start_column, end_column;
};
