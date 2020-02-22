#pragma once
#include <string>

class FileCLI
{
public:
	void readFile();
	void clearFile();
	void appendFile();
	~FileCLI();
private:
	void listFile();
	void getFileID();
	int fileID;
	int offSet;
	int numBytes;
	std::string filePathName;
	std::string textAppend;
};

