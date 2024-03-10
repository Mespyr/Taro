#include <iostream>

#include "error/error.hpp"
#include "scanner/scanner.hpp"
#include "token.hpp"

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No file provided for compilation" << std::endl;
        return 1;
    }

    Scanner scanner(argv[1]);
    if (scanner.error) {
        std::cerr << scanner.error->to_string() << std::endl;
        return 1;
    }

    while (!scanner.eof()) {
        Token t = scanner.next();
        std::cout << t.loc.line_num << ":" << t.loc.start_column << ": "
                  << t.value << std::endl;
    }

    return 0;
}
