#include <iostream>
#include "scanner/scanner.hpp"
#include "token.hpp"

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		std::cerr << "Error: No file provided for compilation" << std::endl;
		return 1;
	}

	Scanner scanner(argv[1]);
	if (scanner.error) {
		std::cout << scanner.error.value().to_string() << std::endl;
		return 1;
	}
	return 0;
}
