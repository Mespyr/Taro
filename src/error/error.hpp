#pragma once

#include <string>
#include "../location.hpp"

class Error {
public:
	Error(std::string message) :
		message(message) {}
	std::string to_string();
private:
	std::string message;
};

class LocatedError : Error {
public:
	LocatedError(Location loc, std::string message) :
		Error(message),
		loc(loc)
	{}
private:
	Location loc;
};
