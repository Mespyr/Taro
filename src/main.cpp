#include <iostream>
#include "include/file.h"

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		std::cerr << "Error: No file provided for compilation" << std::endl;
		return 1;
	}

	File file(argv[1], FILE_READ);

	if (!file.exists())
	{
		std::cerr << "Error: file '" << argv[1] << "' does not exist" << std::endl;
		return 1;
	}

	std::cout << file.read();
	return 0;
}
