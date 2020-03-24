#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <map>
#include "CacheManager/CacheService.h"

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
	std::map<int, std::string> cacheReference;
	std::vector<std::string> cacheKeys;
};

