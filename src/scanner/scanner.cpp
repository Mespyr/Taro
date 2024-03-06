#include "scanner.hpp"

Scanner::Scanner(const std::string& filename) {
	insert(filename);
}

void Scanner::insert(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		error = Error("couldn't open file '" + filename + "': " + strerror(errno));
		return;
	}
	file.close();
}
