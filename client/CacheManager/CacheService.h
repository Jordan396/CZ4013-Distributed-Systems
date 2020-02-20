#pragma once
#include <string>
#include <map>
#include <chrono>
#include "File.h"
class CacheService
{
public:
	bool cacheFile(std::string pathName, char* text, std::chrono::system_clock::time_point time);
	bool clearFile(std::string pathName);
	bool clearCache();
	// will check the freshness as well, if expired or does not exist, return null
	std::string getCachedFile(std::string pathName);

private:
	std::map<std::string, File> cacheMap;
	std::string extractFileName(std::string pathName);
	bool writeFile(std::string pathName, char* text);
	std::string readFile(std::string pathName);
};

