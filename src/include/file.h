#pragma once

#include <fstream>
#include <string>
#include <iostream>

enum FILE_MODE
{
	FILE_READ,
	FILE_WRITE
};

class File
{
public:
	File(std::string filename, FILE_MODE mode);
	~File();
	bool operator==(bool t);

	void writeln(std::string buf);
	std::string read();
	std::string readline();
	bool exists();
private:
	std::fstream file;
};
