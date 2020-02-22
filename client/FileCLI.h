#pragma once
#include <string>

class FileCLI
{
public:
	void readFile();
	void clearFile();
	void writeFile();
	void fetchFile();
	~FileCLI();
private:
	void listFile();
	bool checkValidity(int fileID);
	int cacheSize;
};

