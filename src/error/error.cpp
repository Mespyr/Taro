#include "error.hpp"

std::string Error::to_string() { return "error: " + message; }

std::string LocationError::to_string() {
    std::string err = "┌─ " + loc.file->path() + ":" +
                      std::to_string(loc.line_number() + 1) + ":" +
                      std::to_string(loc.start_col()) + "\n│ " +
                      loc.file->at(loc.line_number()) + "\n│ ";

    for (uint64_t i = 0; i < loc.start_col(); i++) err.push_back(' ');

    for (uint64_t i = loc.start_col(); i < loc.end_col(); i++)
        err.push_back('^');

    return err.append("\n└─ " + message);
}
