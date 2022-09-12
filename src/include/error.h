#ifndef _ACH_ERROR_H_
#define _ACH_ERROR_H_

#include "location.h"
#include "token.h"
#include "types.h"
#include <iostream>
#include <string>
#include <vector>

void print_error(std::string message);
void print_error_at_loc(Location loc, std::string message);
void print_note_at_loc(Location loc, std::string message);
void print_not_enough_arguments_error(Location loc, int expected, int actual, std::string name, std::string fullname = "", bool is_func = false);
void print_invalid_combination_of_types_error(Location loc, std::vector<DATATYPE> types, std::string name, std::string fullname = "", bool is_func = false);
void print_invalid_type_error(Location loc, DATATYPE expected, DATATYPE actual, std::string name, std::string fullname = "", bool is_func = false);

#endif
