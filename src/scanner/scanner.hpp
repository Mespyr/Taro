#pragma once

#include <optional>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
#include "../token.hpp"
#include "../error/error.hpp"

class Scanner {
public:
	std::optional<Error> error;
	Scanner(const std::string& filename);
	void insert(const std::string& filename);
private:
	std::vector<Token> token_stream;
};
