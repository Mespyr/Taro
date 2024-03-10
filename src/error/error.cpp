#include "error.hpp"

std::string Error::to_string() { return "Error: " + message; }

std::string LocationError::to_string() {
    std::string err =
        "┌[" + loc.file_location + ":" + std::to_string(loc.line_num) + ":" +
        std::to_string(loc.start_column) + "]\n│ " + loc.line + "\n└ ";

    for (uint64_t i = 0; i < loc.start_column; i++) {
        if (loc.line.at(i) == '\t')
            err.push_back('\t');
        else
            err.push_back(' ');
    }

    for (uint64_t i = loc.start_column; i < loc.end_column; i++) {
        if (loc.line.at(i) == '\t')
            err.push_back('\t');
        else
            err.push_back('^');
    }

    err.append(" " + message);
    return err;
}
