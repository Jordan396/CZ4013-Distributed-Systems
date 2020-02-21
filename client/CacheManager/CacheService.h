#pragma once
#include <string>
#include <map>
#include <chrono>
#include "File.h"
class CacheService
{
public:
	// this method will be called when client adds a new file from server or write the file and recached it 
	bool cacheFile(std::string pathName, char* text, std::chrono::system_clock::time_point time);

	// this method will be called when client wants to delete a particular cache entry
	bool clearFile(std::string pathName);

	// this method will be called when client wants to clear the whole cache
	bool clearCache();

	// this method will be called when client retrieves the file for writing or reading 
	// if file is outdated with server side or does not exist, return empty string
	std::string getCachedFile(std::string pathName);

	~CacheService();
private:
	std::map<std::string, File> cacheMap;
	std::string extractFileName(std::string pathName);
	bool writeFile(std::string pathName, char* text);
	std::string readFile(std::string pathName);
};

