#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

class File {
  public:
    File(const std::string &file_path);

    std::string at(uint32_t line_number);
    std::string path();
    uint32_t    line_count();
    bool        is_open();

  private:
    bool                     open_success = false;
    std::string              file_path;
    std::vector<std::string> lines;
};
