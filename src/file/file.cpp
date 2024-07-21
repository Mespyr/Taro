#include "file.hpp"

File::File(const std::string &file_path) : file_path(file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) return;

    open_success = true;
    std::string temp_buf;

    while (!file.eof()) {
        std::getline(file, temp_buf);
        lines.push_back(temp_buf);
    }
}

std::string File::at(uint32_t line_number) {
    if (line_number >= line_count()) {}
    return lines.at(line_number);
}

std::string File::path() { return file_path; }
uint32_t    File::line_count() { return lines.size(); }
bool        File::is_open() { return open_success; }
