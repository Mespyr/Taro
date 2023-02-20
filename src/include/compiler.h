#pragma once

#include <string>
#include <assert.h>
#include <sstream>
#include "program.h"
#include "op.h"
#include "file.h"

void compile_to_asm(Program program, std::string output_filename);
