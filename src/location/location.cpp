#include "location.hpp"

uint32_t Location::line_number() { return line_num; }
uint32_t Location::start_col() { return start_column; }
uint32_t Location::end_col() { return end_column; }
