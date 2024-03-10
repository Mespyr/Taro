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
           !is_single_char_token(line.at(column_number)) &&
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
