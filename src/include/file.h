#ifndef _LCP_FILE_H_
#define _LCP_FILE_H_

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

	void writeln(std::string buf);
	std::string read();
	bool exists();
private:
	std::fstream file;
};

#endif
