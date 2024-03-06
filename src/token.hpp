#pragma once

#include <string>
#include "location.hpp"

class Token {
public:
	enum Type {
		WORD, KEYWORD, INT, STRING,
	};

	Token(std::string value, Type type, Location loc) :
		loc(loc),
		value(value),
		type(type)
	{}

	Location loc;
	std::string value;
	Type type;
};
