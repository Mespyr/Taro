#include "error.hpp"

std::string Error::to_string() {
	return "Error: " + message;
}
