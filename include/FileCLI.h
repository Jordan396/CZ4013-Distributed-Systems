#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <map>
#include "CacheManager/CacheService.h"
class CacheService;

class FileCLI
{
public:
	void readFile();
	void clearFile();
	void clearContent();
	void writeFile();
	void fetchFile();
	void clearCache();
	~FileCLI();
	FileCLI( CacheService& cs);
private:
	void listFile();
	bool checkValidity(int fileID);
	int cacheSize;
	CacheService cv;
	std::map<int, std::string> cacheReference;
	std::vector<std::string> cacheKeys;
};

