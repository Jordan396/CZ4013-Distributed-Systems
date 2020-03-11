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
	FileCLI();
private:
	void listFile();
	bool checkValidity(int fileID);
	int cacheSize;
	CacheService cv;
	map<int, std::string> cacheReference;
	vector<std::string> cacheKeys;
};

