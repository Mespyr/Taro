#include <cctype>
#include <cstdint>

#include "scanner.hpp"

uint32_t Scanner::find_start_col(const std::string& line,
                                 uint32_t           column_number) {
    while (column_number < line.length() &&
           std::isspace(line.at(column_number)) &&
           line.at(column_number) != '#')
        column_number++;
    return column_number;
}

uint32_t Scanner::find_end_col(const std::string& line,
                               uint32_t           column_number) {
    while (column_number < line.length() &&
           !std::isspace(line.at(column_number)) &&
           !Token::single_chars.count(line.at(column_number)) &&
           line.at(column_number) != '#')
        column_number++;
    return column_number;
}

uint32_t Scanner::find_string_end(const std::string& line,
                                  uint32_t           column_number) {
    column_number++;
    while (column_number < line.length() && line.at(column_number) != '"') {
        if (line.at(column_number) == '\\') column_number++;
        column_number++;
    }
    return column_number;
}

uint32_t Scanner::find_char_end(const std::string& line,
                                uint32_t           column_number) {
    column_number++;
    if (column_number >= line.length()) return column_number;
    if (line.at(column_number) == '\\') {
        column_number++;
        if (column_number >= line.length()) return column_number;
    }
    column_number++;
    return column_number;
}
