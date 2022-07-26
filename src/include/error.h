#ifndef _ACH_ERROR_H_
#define _ACH_ERROR_H_

#include "location.h"
#include "token.h"
#include <iostream>
#include <string>
#include <vector>

void print_error(std::string message);
void print_error_at_loc(Location loc, std::string message);

#endif
