#include "include/file.h"

File::File(std::string filename, FILE_MODE mode)
{
	if (mode == FILE_WRITE) 
		file.open(filename, std::ios::out);
	else file.open(filename, std::ios::in);
}

File::~File()
{
	file.close();
}

void File::writeln(std::string buf)
{
	buf.push_back('\n');
	file.write(buf.c_str(), buf.length());
}

std::string File::read()
{
	std::string contents;
	std::string line;
	while (file) contents.push_back(file.get());

	return contents;
}

bool File::exists()
{
	if (!file)
		return false;
	return true;
}
