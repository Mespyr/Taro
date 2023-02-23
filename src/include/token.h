#pragma once

#include <string>
#include "location.h"

enum TokenType {
	TOKEN_WORD,
	TOKEN_INT,
	TOKEN_STRING,
	TOKEN_TYPE_COUNT
};

class Token {
public:
	Token(std::string value, TokenType type, Location loc) :
		value(value),
		type(type),
		loc(loc)
	{}

	std::string value;
	TokenType type;
	Location loc;
};
