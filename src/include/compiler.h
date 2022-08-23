#ifndef _LCP_COMPILER_H_
#define _LCP_COMPILER_H_

#include <string>
#include <assert.h>
#include <sstream>
#include "program.h"
#include "op.h"
#include "file.h"

void compile_to_asm(Program program, std::string output_filename);

#endif
