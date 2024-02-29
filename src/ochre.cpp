#include <iostream>

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		std::cerr << "Error: No file provided for compilation" << std::endl;
		return 1;
	}

	std::cout << argv[1] << std::endl;
	return 0;
}
